//! TBD.

use crate::common::ErrorResponder;
use crate::entities::{prelude::*, *};
use crate::model::user::{SignUpRequestData, UserSettings};
use crate::sec::hash_password;

use base64::{Engine as _, engine::general_purpose};
use rocket::{http::CookieJar, State, serde::json::Json};
use serde_json::{json, Value};
use sea_orm::*;

/// Creates a new user.
#[post("/sign-up", format = "json", data = "<data>")]
pub async fn new_user(data: Json<SignUpRequestData>, db: &State<DatabaseConnection>) -> Result<Value, ErrorResponder> {
  if data.phash.len() != 64 { return Err("SHA3-256 hash must have the length of 64.".into()); }
  let settings = UserSettings { name: data.name.clone(), ..Default::default() };
  let p_hash = hash_password(&data.phash);
  let new_user = user::ActiveModel { 
    login: ActiveValue::Set(data.login.clone()),
    p_hash: ActiveValue::Set(general_purpose::STANDARD.encode(p_hash)),
    settings: ActiveValue::Set(serde_json::to_string(&settings)?),
    ..Default::default()
  };
  let db = db as &DatabaseConnection;
  let res = User::insert(new_user).exec(db).await?;
  Ok(json!({ "id": res.last_insert_id }))
}

/// Removes the user.
/// WARNING The safety thoughts: if anyone steal your token, he can delete your account even if he can't decrypt it.
/// A solution: use Redis to store 14-days temporary tokens and save them into private cookies.
#[delete("/user")]
pub async fn remove_user(jar: &CookieJar<'_>, db: &State<DatabaseConnection>) -> Result<(), ErrorResponder> {
  let user_id: i64 = jar
    .get_private("user_id")
    .ok_or::<String>("You haven't logged in.".into())?
    .value()
    .parse::<i64>()?;
  let db = db as &DatabaseConnection;
  let _ = User::delete_by_id(user_id).exec(db).await?;
  Ok(())
}
