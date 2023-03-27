//! Security functions.

use crate::model::{MResult, RedisPool};

use bb8_redis::redis::{AsyncCommands, LposOptions};
use chrono::{DateTime, Duration, Utc, serde::ts_seconds};
use passwords::PasswordGenerator;
use rocket::{State, http::{Cookie, CookieJar}};
use serde::{Deserialize, Serialize};
use sha3::{Digest, Sha3_256};

const TOKEN_LENGTH: usize = 64;
pub const MAX_TOKENS_PER_USER: isize = 3;
pub const TOKEN_COOKIE_NAME: &'static str = "user_token";
pub const DAYS_VALID: i64 = 14;

/// TBD.
#[derive(Deserialize, Serialize)]
pub struct UserToken {
  pub user_id: i64,
  pub token_str: String,
  #[serde(with = "ts_seconds")]
  pub birth: DateTime<Utc>,
}

/// Hashes the pass.
pub fn hash_password(pass: &Vec<u8>) -> Vec<u8> {
  let mut hasher = Sha3_256::new();
  hasher.update(pass);
  hasher.finalize().to_vec()
}

/// Checks if the `phash` field is the 64-bytes vec.
pub fn check_hash(phash: &Vec<u8>) -> MResult<()> {
  if phash.len() != 64 { return Err("SHA3-256 hash must have the length of 64.".into()); }
  Ok(())
}

/// TBD.
pub fn validate_hashes(phash_u: &Vec<u8>, phash_db: &Vec<u8>) -> MResult<()> {
  if hash_password(phash_u).eq(phash_db) { return Ok(()); }
  else { return Err("Wrong password.".into()) }
}

/// Returns the name of list where user tokens is stored into.
pub fn get_user_tokens_list_name(user_id: &i64) -> String {
  format!("user_tokens:id{}", user_id)
}

/// TBD.
pub async fn check_user(jar: &CookieJar<'_>, cacher: &State<RedisPool>) -> MResult<i64> {
  let user_token_str = jar
    .get_private(TOKEN_COOKIE_NAME)
    .ok_or::<String>("You haven't logged in.".into())?
    .value().to_owned();
  let user_token = serde_json::from_str::<UserToken>(&user_token_str)?;
  let user_tokens_list = get_user_tokens_list_name(&user_token.user_id);
  let cacher = cacher as &RedisPool;
  let mut cacher_conn = cacher.get().await?;
  let idx: Option<i32> = cacher_conn.lpos(&user_tokens_list, &user_token_str, LposOptions::default()).await?;
  if idx.is_none() {
    jar.remove_private(Cookie::named(TOKEN_COOKIE_NAME));
    return Err("There is no such tokens.".into())
  }
  let duration: Duration = Utc::now() - user_token.birth;
  if duration.num_days() >= DAYS_VALID {
    cacher_conn.lrem(user_tokens_list, 1, user_token_str).await?;
    jar.remove_private(Cookie::named(TOKEN_COOKIE_NAME));
    return Err("The token is expired.".into())
  }
  Ok(user_token.user_id)
}

/// TBD.
pub fn generate_token(user_id: i64) -> MResult<UserToken> {
  let pg = PasswordGenerator {
    length: TOKEN_LENGTH,
    numbers: true,
    lowercase_letters: true,
    uppercase_letters: true,
    symbols: true,
    strict: true,
    exclude_similar_characters: true,
    spaces: false,
  };
  Ok(UserToken {
    user_id,
    token_str: pg.generate_one()?,
    birth: Utc::now(),
  })
}
