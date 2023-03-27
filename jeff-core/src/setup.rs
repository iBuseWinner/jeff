//! TDB.

use crate::model::MResult;
use crate::model::{DbPool, RedisPool};

use bb8_redis::{bb8::Pool as Bb8Pool, RedisConnectionManager};
use sea_orm::*;
use std::{env, fs, io::Read, net::SocketAddr};
use serde::Deserialize;

#[derive(Deserialize)]
pub struct AppState {
  pub db_url: String,
  pub redis_url: String,
  pub admin_key: String,
  pub rocket_addr: SocketAddr,
}

const DB_NAME: &str = "jeff_core";

pub(super) async fn load_app_state() -> MResult<(AppState, DbPool, RedisPool)> {
  let filepath = env::args().nth(1).unwrap();
  let mut file = fs::File::open(filepath)?;
  let mut buffer = String::new();
  file.read_to_string(&mut buffer)?;
  let mut state: AppState = serde_json::from_str(&buffer)?;
  let (db, new_url) = connect_db(&state.db_url).await?;
  let cacher = connect_redis(&state.redis_url).await?;
  if let Some(url) = new_url { state.db_url = url; };
  match state.admin_key.len() < 64 {
    true => Err("Administrator key length less than 64.".into()),
    false => Ok((state, db, cacher)),
  }
}

pub(crate) async fn connect_db(db_url: &str) -> MResult<(DbPool, Option<String>)> {
  let db = Database::connect(db_url).await?;
  match db.get_database_backend() {
    DbBackend::MySql | DbBackend::Postgres => {
      let db_url = format!("{}/{}", &db_url, DB_NAME);
      Ok((Database::connect(&db_url).await?, Some(db_url)))
    },
    DbBackend::Sqlite => Ok((db, None)),
  }
}

async fn connect_redis(redis_url: &str) -> MResult<RedisPool> {
  let manager = RedisConnectionManager::new(redis_url)?;
  Ok(Bb8Pool::builder().build(manager).await?)
}
