#[macro_use] extern crate rocket;

mod common;
mod setup;
mod types;

use rocket::tokio::time::{sleep, Duration};
use rocket::serde::json::Json;
use crate::types::SignUpRequestData;

#[get("/")]
fn index() -> &'static str {
  "Jeff is running smoothly. Have a nice day!\n\nhttps://github.com/markcda/jeff"
}

#[post("/sign-up", format = "json", data = "<data>")]
async fn new_user(data: Json<SignUpRequestData>) {
  unimplemented!();
}


#[launch]
async fn rocket() -> _ {
  let (state, db) = match setup::load_app_state().await {
    Ok((state, db)) => (state, db),
    Err(err) => panic!("{}", err),
  };
  rocket::build()
    .manage(state)
    .manage(db)
    .mount("/", routes![index, new_user])
}
