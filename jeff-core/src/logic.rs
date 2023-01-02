use chrono::{Utc, Duration};
use core::marker::{Send, Sync};
use custom_error::custom_error;
use futures::future;
use hyper::Body;
use hyper::http::Response;
use serde_json::Value as JsonValue;
use sha3::{Digest, Sha3_256};
use std::{boxed::Box, collections::HashSet};
use tokio_postgres::types::ToSql;

use crate::model::{extract, extract_creds, Workspace, Db, AdminCredentials, UserCredentials, Token, TokenAuth};

type MResult<T> = Result<T, Box<dyn std::error::Error>>;

custom_error!{NFO{}  = "Failed to get data."}
custom_error!{WDE{}  = "Failed to write data."}

// Response generating functions.

pub fn from_code_and_msg(code: u16, msg: Option<&str>) -> Response<Body> {
  Response::builder()
    .header("Content-Type", "text/html; charset=utf-8")
    //.header("Access-Control-Allow-Origin", "http://localhost:3000")
    //.header("Access-Control-Allow-Credentials", "true")
    .status(code)
    .body(match msg {
      None => Body::empty(),
      Some(msg) => Body::from(String::from(msg)),
    })
    .unwrap()
}

// pub fn options_answer() -> Response<Body> {
//   Response::builder()
//     .header("Access-Control-Allow-Origin", "http://localhost:3000")
//     .header("Access-Control-Allow-Credentials", "true")
//     .header("Access-Control-Allow-Methods", "GET, POST, PUT, PATCH, DELETE, OPTIONS")
//     .header("Access-Control-Allow-Headers", "App-Token")
//     .body(Body::empty())
//     .unwrap()
// }

// Common functions.

pub async fn get_tokens(db: &Db, id: &i64) -> MResult<Vec<Token>> {
  let user_data = db.read("select user_creds from users where id = $1;", &[id]).await?;
  let user_credentials: UserCredentials = serde_json::from_str(user_data.get(0))?;
  Ok(user_credentials.tokens)
}

pub async fn write_tokens(db: &Db, id: &i64, tokens: &Vec<Token>) -> MResult<()> {
  let user_credentials = db.read("select user_creds from users where id = $1;", &[id]).await?;
  let mut user_credentials: UserCredentials = serde_json::from_str(user_credentials.get(0))?;
  user_credentials.tokens = tokens.clone();
  let user_credentials = serde_json::to_string(&user_credentials)?;
  db.write("update users set user_creds = $1 where id = $2;", &[&user_credentials, id]).await
}

pub async fn verify_user(db: &Db, token_auth: &TokenAuth) -> bool {
  let mut tokens = get_tokens(db, &token_auth.id).await.unwrap();
  let mut s: usize = 0;
  let mut i: usize = 0;
  let mut validated: bool = false;
  while s + i < tokens.len() {
    if s > 0 {
      tokens[i].tk = tokens[i + s].tk.clone();
      tokens[i].from_dt = tokens[i + s].from_dt;
    }
    let duration: Duration = Utc::now() - tokens[i].from_dt;
    if duration.num_days() >= 5 {
      s += 1;
    } else {
      let mut hasher = Sha3_256::new();
      hasher.update(&token_auth.token);
      let hashed = hasher.finalize();
      if tokens[i].tk == hashed.to_vec() {
        validated = true;
        tokens[i].from_dt = Utc::now();
      }
      i += 1;
    }
  }
  tokens.truncate(tokens.len() - s);
  if (s > 0) || validated {
    match write_tokens(db, &token_auth.id, &tokens).await {
      Err(_) => false,
      Ok(_) => validated,
    }
  } else {
    validated
  }
}

// Application logic's functions.

pub async fn db_setup(ws: Workspace, admin_key: String) -> Response<Body> {
  let key = match extract_creds::<AdminCredentials>(ws.req.headers().get("App-Token")) {
    Ok(v) => v.key,
    _ => return from_code_and_msg(401, Some("No valid token received.")),
  };
  let status_code = match key == admin_key {
    true => match ws.db.write_mul(vec![
      ("create table if not exists taskboard_keys (key varchar unique, value varchar);", vec![]),
      ("create table if not exists users (id bigserial, login varchar unique, sources varchar, user_creds varchar, scripts varchar);", vec![]),
      ("create table if not exists sources (id bigserial, shared_with varchar, header varchar);", vec![]),
      ("create table if not exists scripts (id bigserial, shared_with varchar, path varchar);", vec![]),
      ("create table if not exists hists (id bigserial, author bigint, header varchar);", vec![]),
      ("create table if not exists id_seqs (id varchar unique, val bigint);", vec![])
    ]).await {
      Ok(_) => 200,
      _ => 500,
    },
    _ => 401,
  };
  from_code_and_msg(status_code, None)
}

pub async fn sign_up(ws: Workspace) -> Response<Body> {
  unimplemented!();
}

pub async fn sign_in(ws: Workspace) -> Response<Body> {
  unimplemented!();
}

pub async fn auth_by_token(ws: &Workspace) -> Result<i64, (u16, String)> {
  unimplemented!();
}

pub async fn delete_account(ws: Workspace, user_id: i64) -> Response<Body> {
  unimplemented!();
}

pub async fn patch_user_creds(ws: Workspace, user_id: i64) -> Response<Body> {
  unimplemented!();
}

pub async fn new_source(ws: Workspace, user_id: i64) -> Response<Body> {
  unimplemented!();
}

pub async fn import_to_source(ws: Workspace, user_id: i64) -> Response<Body> {
  unimplemented!();
}

pub async fn export_source(ws: Workspace, user_id: i64) -> Response<Body> {
  unimplemented!();
}

pub async fn new_expression(ws: Workspace, user_id: i64) -> Response<Body> {
  unimplemented!();
}

pub async fn patch_expression(ws: Workspace, user_id: i64) -> Response<Body> {
  unimplemented!();
}

pub async fn delete_expression(ws: Workspace, user_id: i64) -> Response<Body> {
  unimplemented!();
}

pub async fn get_answer(ws: Workspace, user_id: i64) -> Response<Body> {
  unimplemented!();
}

pub async fn get_memory_cell(ws: Workspace, user_id: i64) -> Response<Body> {
  unimplemented!();
}

pub async fn put_memory_cell(ws: Workspace, user_id: i64) -> Response<Body> {
  unimplemented!();
}

pub async fn get_history(ws: Workspace, user_id: i64) -> Response<Body> {
  unimplemented!();
}

pub async fn clear_history(ws: Workspace, user_id: i64) -> Response<Body> {
  unimplemented!();
}
