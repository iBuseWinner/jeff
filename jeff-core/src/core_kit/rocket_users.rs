//! TBD.

use crate::entities::{prelude::*, *};
use crate::model::{user::{SignUpRequestData, SignInRequestData, UserSettings}, DbPool, RedisPool, MResult};
use crate::sec::*;

use base64::{Engine as _, engine::general_purpose};
use bb8_redis::redis::AsyncCommands;
use rocket::{http::{Cookie, CookieJar}, State, serde::json::Json};
// use serde_json::{json, Value};
use sea_orm::*;

/// Creates a new user.
#[put("/user", format = "json", data = "<data>")]
pub async fn new_user(data: Json<SignUpRequestData>, db: &State<DbPool>) -> MResult<()> {
  check_hash(&data.phash)?;
  let settings = UserSettings { name: data.name.clone(), ..Default::default() };
  let p_hash = hash_password(&data.phash);
  let new_user = user::ActiveModel { 
    login: ActiveValue::Set(data.login.clone()),
    p_hash: ActiveValue::Set(general_purpose::STANDARD.encode(p_hash)),
    settings: ActiveValue::Set(serde_json::to_string(&settings)?),
    ..Default::default()
  };
  User::insert(new_user).exec(db as &DbPool).await?;
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
  let user_data: Option<user::Model> = User::find()
    .filter(user::Column::Login.eq(&data.login))
    .one(db as &DbPool)
    .await?;
  let user_id: i64 = match user_data {
    None => return Err("There is no such user.".into()),
    Some(user_data) => {
      let phash_db = general_purpose::STANDARD.decode(user_data.p_hash.as_bytes())?;
      validate_hashes(&data.phash, &phash_db)?;
      user_data.id
    },
  };
  // Step 2. Generates a token and stores it into cacher.
  let utl_name = get_user_tokens_list_name(&user_id);
  let cacher = cacher as &RedisPool;
  let mut cacher_conn = cacher.get().await?;
  let user_tokens_list_len: isize = cacher_conn.llen(&utl_name).await?;
  let token = generate_token(user_id)?;
  let token = serde_json::to_string(&token)?;
  if user_tokens_list_len >= MAX_TOKENS_PER_USER { cacher_conn.ltrim(&utl_name, 0, MAX_TOKENS_PER_USER - 1).await?; }
  cacher_conn.lpush(&utl_name, &token).await?;
  // Step 3. Sets the token as private user cookie.
  jar.remove_private(Cookie::named(TOKEN_COOKIE_NAME));
  jar.add_private(Cookie::new(TOKEN_COOKIE_NAME, token));
  Ok(())
}

/// Removes the user.
#[delete("/user")]
pub async fn remove_user(jar: &CookieJar<'_>, db: &State<DbPool>, cacher: &State<RedisPool>) -> MResult<()> {
  let user_id = check_user(jar, cacher).await?;
  let _ = User::delete_by_id(user_id).exec(db as &DbPool).await?;
  let cacher = cacher as &RedisPool;
  let mut cacher_conn = cacher.get().await?;
  let utl_name = get_user_tokens_list_name(&user_id);
  cacher_conn.del(utl_name).await?;
  jar.remove_private(Cookie::named(TOKEN_COOKIE_NAME));
  Ok(())
}
