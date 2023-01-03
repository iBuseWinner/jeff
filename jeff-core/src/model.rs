//! Application Data Model.

use bb8::Pool;
use bb8_postgres::PostgresConnectionManager as PgConManager;
use chrono::{DateTime, Utc, Duration, serde::ts_seconds};
use core::marker::{Send, Sync};
use custom_error::custom_error;
use futures::future;
use hyper::{Body, body::to_bytes, http::Request};
use passwords::{PasswordGenerator, hasher::{bcrypt, gen_salt}};
use serde::{Deserialize, Serialize, de::DeserializeOwned};
use sha3::{Digest, Sha3_256};
use std::net::SocketAddr;
use tokio_postgres::{ToStatement, types::ToSql, row::Row, NoTls};

type MResult<T> = Result<T, Box<dyn std::error::Error>>;

// Database definition.

#[derive(Clone)]
pub struct Db {
  pool: Pool<PgConManager<NoTls>>,
}

impl Db {
  pub fn new(pool: Pool<PgConManager<NoTls>>) -> Db {
    Db { pool }
  }

  pub async fn read<T>(&self, statement: &T, params: &[&(dyn ToSql + Sync)]) -> MResult<Row>
  where T: ?Sized + ToStatement {
    let cli = self.pool.get().await?;
    Ok(cli.query_one(statement, params).await?)
  }
  
  pub async fn write<T>(&self, statement: &T, params: &[&(dyn ToSql + Sync)]) -> MResult<()>
  where T: ?Sized + ToStatement {
    let mut cli = self.pool.get().await?;
    let tr = cli.transaction().await?;
    tr.execute(statement, params).await?;
    tr.commit().await?;
    Ok(())
  }
  
  pub async fn read_mul<T>(&self, parts: Vec<(&T, Vec<&(dyn ToSql + Sync)>)>) -> MResult<Vec<Row>>
  where T: ?Sized + ToStatement + Send + Sync {
    let cli = self.pool.get().await?;
    let mut tasks = Vec::new();
    for i in 0..parts.len() {
      tasks.push(cli.query_one(parts[i].0, &parts[i].1));
    };
    let results = future::try_join_all(tasks).await?;
    Ok(results)
  }
  
  pub async fn write_mul<T>(&self, parts: Vec<(&T, Vec<&(dyn ToSql + Sync)>)>) -> MResult<()>
  where T: ?Sized + ToStatement + Send + Sync {
    let mut cli = self.pool.get().await?;
    let tr = cli.transaction().await?;
    let mut tasks = Vec::new();
    for i in 0..parts.len() {
      tasks.push(tr.execute(parts[i].0, &parts[i].1));
    };
    future::try_join_all(tasks).await?;
    tr.commit().await?;
    Ok(())
  }
}

// Common structs.

pub struct Workspace {
  pub req: Request<Body>,
  pub db: Db,
}

#[derive(Deserialize, Serialize)]
pub struct User {
  pub id: i64,
  pub shared_boards: Vec<i64>,
  pub user_creds: UserCredentials,
}

#[derive(Deserialize, Serialize, Clone)]
pub struct AppConfig {
  pub pg: String,
  pub admin_key: String,
  pub hyper_addr: SocketAddr,
  pub daemons: Vec<Daemon>,
}

#[derive(Deserialize, Serialize, Clone)]
pub struct Daemon {
  pub cmd: String,
  pub server: Option<SocketAddr>,
}

pub struct DaemonsHolder {
  pub spawned: Vec<(Daemon, std::io::Result<tokio::process::Child>)>,
}

// App security's structs.

#[derive(Deserialize, Serialize)]
pub struct AdminCredentials {
  pub key: String,
}

#[derive(Deserialize, Serialize, Clone)]
pub struct TokenAuth {
  pub id: i64,
  pub token: String,
}

#[derive(Deserialize, Serialize, Clone)]
pub struct Token {
  pub tk: Vec<u8>,
  #[serde(with = "ts_seconds")]
  pub from_dt: DateTime<Utc>,
}

#[derive(Deserialize, Serialize)]
pub struct SignInCredentials {
  pub login: String,
  pub pass: String,
}

#[derive(Deserialize, Serialize)]
pub struct SignUpCredentials {
  pub login: String,
  pub pass: String,
}

#[derive(Deserialize, Serialize)]
pub struct UserCredentials {
  pub salt: Vec<u8>,
  pub salted_pass: Vec<u8>,
  pub tokens: Vec<Token>,
}

// Extractors.

custom_error!{ pub ExtractionError
  FromBody = "Failed to get data from request body.",
  FromHeader = "Failed to get data from request header.",
  FromBytes = "Failed to create string from request body bytes list.",
  FromBase64 = "Failed to decode data from base64.",
  FromJson = "Failed to deserialize JSON."
}

pub async fn extract<T>(req: Request<Body>) -> Result<T, ExtractionError>
  where
    T: DeserializeOwned,
{
  let body = match to_bytes(req.into_body()).await {
    Err(_) => return Err(ExtractionError::FromBody),
    Ok(v) => v,
  };
  let body = match String::from_utf8(body.to_vec()) {
    Err(_) => return Err(ExtractionError::FromBytes),
    Ok(v) => v.clone(),
  };
  match serde_json::from_str::<T>(&body) {
    Err(_) => Err(ExtractionError::FromJson),
    Ok(v) => Ok(v),
  }
}

pub fn extract_creds<T>(header: Option<&hyper::header::HeaderValue>) -> Result<T, ExtractionError> 
  where
    T: DeserializeOwned,
{
  let creds = match header {
    None => return Err(ExtractionError::FromHeader),
    Some(v) => v,
  };
  let creds = match creds.to_str() {
    Err(_) => return Err(ExtractionError::FromHeader),
    Ok(v) => String::from(v),
  };
  let creds = match base64::decode(&creds) {
    Err(_) => return Err(ExtractionError::FromBase64),
    Ok(v) => match String::from_utf8(v) {
      Err(_) => return Err(ExtractionError::FromBytes),
      Ok(v) => v,
    },
  };
  match serde_json::from_str::<T>(&creds) {
    Err(_) => return Err(ExtractionError::FromJson),
    Ok(v) => Ok(v),
  }
}

// Password's generator and checker.

pub fn generate_strong(length: usize) -> Result<String, &'static str> {
  let pg = PasswordGenerator {
    length,
    numbers: true,
    lowercase_letters: true,
    uppercase_letters: true,
    symbols: true,
    strict: true,
    exclude_similar_characters: true,
    spaces: false,
  };
  Ok(pg.generate_one()?)
}

pub fn salt_pass(pass: String) -> Result<(Vec<u8>, Vec<u8>), &'static str> {
  let salt = Vec::from(gen_salt());
  let salted_pass = Vec::from(bcrypt(10, &salt, &pass)?);
  Ok((salt, salted_pass))
}

pub fn check_pass(salt: Vec<u8>, salted_pass: Vec<u8>, guessed_pass: &String) -> bool {
  Vec::from(bcrypt(10, &salt, &guessed_pass).unwrap()) == salted_pass
}
