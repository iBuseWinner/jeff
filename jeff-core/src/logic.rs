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

use crate::model::{
  extract, extract_creds, salt_pass, generate_strong, check_pass, Workspace, Db, AdminCredentials,
  SignUpCredentials, SignInCredentials, UserCredentials, Token, TokenAuth
};

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
      ("create table if not exists users (id bigserial, login varchar unique, user_creds varchar, sources varchar, scripts varchar);", vec![]),
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

pub async fn create_user(db: &Db, sign_up_credentials: &SignUpCredentials) -> MResult<i64> {
  let (salt, salted_pass) = salt_pass(sign_up_credentials.pass.clone())?;
  let id: i64 = db.read("select nextval(pg_get_serial_sequence('users', 'id'));", &[]).await?.get(0);
  let user_credentials = UserCredentials { salt, salted_pass, tokens: vec![] };
  let user_credentials = serde_json::to_string(&user_credentials)?;
  db.write(
    "insert into users values ($1, $2, $3, '[]', '[]');",
    &[&id, &sign_up_credentials.login, &user_credentials]
  ).await?;
  Ok(id)
}

pub async fn get_new_token(db: &Db, id: &i64) -> MResult<TokenAuth> {
  let user_credentials = db.read("select user_creds from users where id = $1;", &[id]).await?;
  let mut user_credentials: UserCredentials = serde_json::from_str(user_credentials.get(0))?;
  let token = generate_strong(64)?;
  let mut hasher = Sha3_256::new();
  hasher.update(&token);
  let hashed = hasher.finalize();
  let token_info = Token {
    tk: hashed.to_vec(),
    from_dt: Utc::now(),
  };
  user_credentials.tokens.push(token_info.clone());
  let user_credentials = serde_json::to_string(&user_credentials)?;
  db.write("update users set user_creds = $1 where id = $2;", &[&user_credentials, id]).await?;
  let token_auth = TokenAuth { id: *id, token: token };
  Ok(token_auth)
}

pub async fn sign_up(ws: Workspace) -> Response<Body> {
  let su_creds = match extract_creds::<SignUpCredentials>(ws.req.headers().get("App-Token")) {
    Ok(v) => v,
    _ => return from_code_and_msg(401, Some("No valid token received.")),
  };
  if su_creds.pass.len() < 8 {
    return from_code_and_msg(400, Some("Password is too short"));
  };
  let id = match create_user(&ws.db, &su_creds).await {
    Ok(v) => v,
    _ => return from_code_and_msg(500, Some("Unable to create user - internal error.")),
  };
  match get_new_token(&ws.db, &id).await {
    Ok(token_auth) => from_code_and_msg(200, Some(&serde_json::to_string(&token_auth).unwrap())),
    _ => from_code_and_msg(500, Some("Unable to create error - internal error.")),
  }
}

pub async fn sign_in_creds_to_id(db: &Db, sign_in_credentials: &SignInCredentials) -> MResult<i64> {
  custom_error!{IncorrectPassword{} = "Wrong password!"};
  let id_and_credentials = db.read(
    "select id, user_creds from users where login = $1;", &[&sign_in_credentials.login]
  ).await?;
  let user_credentials: UserCredentials = serde_json::from_str(id_and_credentials.get(1))?;
  match check_pass(
    user_credentials.salt,
    user_credentials.salted_pass,
    &sign_in_credentials.pass
  ) {
    true => Ok(id_and_credentials.get(0)),
    _ => Err(Box::new(IncorrectPassword{})),
  }
}

pub async fn sign_in(ws: Workspace) -> Response<Body> {
  let si_creds = match extract_creds::<SignInCredentials>(ws.req.headers().get("App-Token")) {
    Ok(v) => v,
    _ => return from_code_and_msg(401, Some("No valid App-Token received.")),
  };
  let id = match sign_in_creds_to_id(&ws.db, &si_creds).await {
    Ok(v) => v,
    _ => return from_code_and_msg(401, None),
  };
  let token_auth = match get_new_token(&ws.db, &id).await {
    Ok(v) => v,
    _ => return from_code_and_msg(500, None),
  };
  match serde_json::to_string(&token_auth) {
    Ok(body) => from_code_and_msg(200, Some(&body)),
    _ => from_code_and_msg(500, None),
  }
}

pub async fn auth_by_token(ws: &Workspace) -> Result<i64, (u16, String)> {
  let token_auth = match extract_creds::<TokenAuth>(ws.req.headers().get("App-Token")) {
    Ok(v) => v,
    _ => return Err((401, "No valid App-Token received.".into())),
  };
  let valid = verify_user(&ws.db, &token_auth).await;
  if !valid {
    return Err((401, "Wrong token, please, sign in again.".into()));
  };
  Ok(token_auth.id)
}

pub async fn delete_account(ws: Workspace, user_id: i64) -> Response<Body> {
  match ws.db.write("delete from users where id = $1;", &[&user_id]).await {
    Ok(_) => from_code_and_msg(200, None),
    Err(_) => from_code_and_msg(500, Some("Unable to delete user - internal error.")),
  }
}

pub async fn change_password(db: &Db, user_id: &i64, password: &str) -> MResult<()> {
  unimplemented!();
}

pub async fn patch_user_creds(ws: Workspace, user_id: i64) -> Response<Body> {
  let patch = match extract::<JsonValue>(ws.req).await {
    Ok(v) => v,
    _ => return from_code_and_msg(400, Some("Unable to deserialize data.")),
  };
  let password = match patch.get("password") {
    Some(password) => match password.as_str() {
      Some(password) => password,
      _ => return from_code_and_msg(400, Some("Password field must be a string.")),
    },
    _ => return from_code_and_msg(400, Some("No new password received.")),
  };
  match change_password(&ws.db, &user_id, &password).await {
    Ok(_) => from_code_and_msg(200, None),
    _ => from_code_and_msg(500, Some("Не удалось применить патч к доске.")),
  }
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
