//! Rocket routes to work with user data.

use crate::entities::{prelude::*, *};
use crate::model::{user::{
  SignUpRequestData,
  SignInRequestData,
  ChangePasswordRequestData,
  UserSettings,
}, DbPool, RedisPool, MResult};
use crate::sec::*;

use base64::{Engine as _, engine::general_purpose::STANDARD as base64};
use bb8_redis::redis::AsyncCommands;
use rocket::{http::{Cookie, CookieJar}, State, serde::json::Json};
use rocket::{post, put, patch, get, delete};
use sea_orm::*;

/// Finds a user entry by id.
async fn find_by_id(user_id: &i64, db: &State<DbPool>) -> MResult<users::Model> {
  let user_data: Option<users::Model> = Users::find()
    .filter(users::Column::Id.eq(*user_id))
    .one(db as &DbPool)
    .await?;
  Ok(user_data.ok_or::<String>("There is no such user.".into())?)
}

/// Finds a user entry by login.
async fn find_by_login(login: &str, db: &State<DbPool>) -> MResult<users::Model> {
  let user_data: Option<users::Model> = Users::find()
    .filter(users::Column::Login.eq(login))
    .one(db as &DbPool)
    .await?;
  Ok(user_data.ok_or::<String>("There is no such user.".into())?)
}

/// Creates a new user.
#[put("/user", format = "json", data = "<data>")]
pub async fn new_user(data: Json<SignUpRequestData>, db: &State<DbPool>) -> MResult<()> {
  check_hash(&data.phash)?;
  let settings = UserSettings { name: data.name.clone(), ..Default::default() };
  let p_hash = hash_password(&data.phash);
  let new_user = users::ActiveModel { 
    login: ActiveValue::Set(data.login.clone()),
    p_hash: ActiveValue::Set(base64.encode(p_hash)),
    settings: ActiveValue::Set(serde_json::to_string(&settings)?),
    ..Default::default()
  };
  Users::insert(new_user).exec(db as &DbPool).await?;
  Ok(())
}

/// Logs into account.
/// Security thoughts: using Redis/Dragonfly DB, we can actually store new random tokens and save it to private cookies.
/// Another thought: no more than 3 tokens allowed and token length is 64 symbols (checks the consts in `crate::sec`).
#[post("/login", format = "json", data = "<data>")]
pub async fn login(
  data: Json<SignInRequestData>,
  db: &State<DbPool>,
  cacher: &State<RedisPool>,
  jar: &CookieJar<'_>,
) -> MResult<()> {
  // Step 1. Checks the data in DB.
  // Substeps:
  //     1.1. Searches an entry with the same `login`.
  //     1.2. Checks if password hashes are equal.
  //     1.3. Takes the user id.
  check_hash(&data.phash)?;
  let user_data = find_by_login(&data.login, db).await?;
  let phash_db = base64.decode(user_data.p_hash.as_bytes())?;
  validate_hashes(&data.phash, &phash_db)?;
  // Step 2. Generates a token and stores it into cacher.
  let utl_name = get_user_tokens_list_name(&user_data.id);
  let cacher = cacher as &RedisPool;
  let mut cacher_conn = cacher.get().await?;
  let user_tokens_list_len: isize = cacher_conn.llen(&utl_name).await?;
  let token = generate_token(user_data.id)?;
  let token = serde_json::to_string(&token)?;
  if user_tokens_list_len >= MAX_TOKENS_PER_USER { cacher_conn.ltrim(&utl_name, 0, MAX_TOKENS_PER_USER - 1).await?; }
  cacher_conn.lpush(&utl_name, &token).await?;
  // Step 3. Sets the token as private user cookie.
  jar.remove_private(Cookie::named(TOKEN_COOKIE_NAME));
  jar.add_private(Cookie::new(TOKEN_COOKIE_NAME, token));
  Ok(())
}

/// Changes user's password.
#[patch("/user/password", format = "json", data = "<data>")]
pub async fn change_password(
  data: Json<ChangePasswordRequestData>,
  db: &State<DbPool>,
  cacher: &State<RedisPool>,
  jar: &CookieJar<'_>,
) -> MResult<()> {
  check_hash(&data.old_phash)?;
  check_hash(&data.new_phash)?;
  let user_id = check_user(jar, cacher).await?;
  let user_data = find_by_id(&user_id, db).await?;
  let old_phash_db = base64.decode(user_data.p_hash.as_bytes())?;
  validate_hashes(&data.old_phash, &old_phash_db)?;
  let new_phash = hash_password(&data.new_phash);
  let mut user_data: users::ActiveModel = user_data.into();
  user_data.p_hash = Set(base64.encode(new_phash));
  user_data.update(db as &DbPool).await?;
  Ok(())
}

/// Common function that removes all tokens from cacher.
async fn delete_tokens(user_id: &i64, cacher: &State<RedisPool>) -> MResult<()> {
  let cacher = cacher as &RedisPool;
  let mut cacher_conn = cacher.get().await?;
  let utl_name = get_user_tokens_list_name(user_id);
  cacher_conn.del(utl_name).await?;
  Ok(())
}

/// Deauthenticates all clients including current.
#[get("/user/deauth_all")]
pub async fn deauth_all_clients(jar: &CookieJar<'_>, cacher: &State<RedisPool>) -> MResult<()> {
  let user_id = check_user(jar, cacher).await?;
  delete_tokens(&user_id, cacher).await?;
  jar.remove_private(Cookie::named(TOKEN_COOKIE_NAME));
  Ok(())
}

/// Removes the user.
#[delete("/user")]
pub async fn remove_user(jar: &CookieJar<'_>, db: &State<DbPool>, cacher: &State<RedisPool>) -> MResult<()> {
  let user_id = check_user(jar, cacher).await?;
  let _ = Users::delete_by_id(user_id).exec(db as &DbPool).await?;
  delete_tokens(&user_id, cacher).await?;
  jar.remove_private(Cookie::named(TOKEN_COOKIE_NAME));
  Ok(())
}
