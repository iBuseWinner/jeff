//! Jeff Core, the server version of Jeff.

#[macro_use] extern crate rocket;

mod common;
mod entities;
mod sec;
mod setup;
mod types;

use common::ErrorResponder;
use entities::{prelude::*, *};
use sec::hash_password;
use types::{SignUpRequestData, UserSettings};

use base64::{Engine as _, engine::general_purpose};
use rocket::{http::CookieJar, State, serde::json::Json};
use serde_json::{json, Value};
use sea_orm::*;

/// Index page with project link.
#[get("/")]
fn index() -> &'static str {
  "Jeff is running smoothly. Have a nice day!\n\nhttps://github.com/markcda/jeff"
}

/// Creates a new user.
#[post("/sign-up", format = "json", data = "<data>")]
async fn new_user(data: Json<SignUpRequestData>, db: &State<DatabaseConnection>) -> Result<Value, ErrorResponder> {
  if data.phash.len() != 64 { return Err("Hash must have the length of 64.".into()); }
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

#[delete("/user")]
async fn remove_user(jar: &CookieJar<'_>, db: &State<DatabaseConnection>) -> Result<(), ErrorResponder> {
  let user_id: i64 = jar
    .get_private("user_id")
    .ok_or::<String>("You haven't logged in.".into())?
    .value()
    .parse::<i64>()?;
  let db = db as &DatabaseConnection;
  let _ = User::delete_by_id(user_id).exec(db).await?;
  Ok(())
}

/// Loads the application state from config file and starts JC up.
#[launch]
async fn rocket() -> _ {
  let (state, db) = match setup::load_app_state().await {
    Ok((state, db)) => (state, db),
    Err(err) => panic!("{}", err),
  };
  rocket::build()
    .manage(state)
    .manage(db)
    .mount("/", routes![index, new_user, remove_user])
}
