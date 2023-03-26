//! Jeff Core, the server version of Jeff.

#[macro_use] extern crate rocket;

mod common;
mod core_kit;
mod entities;
mod model;
mod sec;
mod setup;

use core_kit::users::{new_user, remove_user};

/// Index page with project link.
#[get("/")]
fn index() -> &'static str {
  "Jeff is running smoothly. Have a nice day!\n\nhttps://github.com/markcda/jeff"
}

/// Loads the application state from config file and starts JC up.
#[launch]
async fn rocket() -> _ {
  let (state, db, cacher) = match setup::load_app_state().await {
    Ok((state, db, cacher)) => (state, db, cacher),
    Err(err) => panic!("{}", err),
  };
  rocket::build()
    .manage(state)
    .manage(db)
    .manage(cacher)
    .mount("/", routes![index, new_user, remove_user])
}
