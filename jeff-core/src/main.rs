//! Jeff Core, the server version of Jeff.

mod core_kit;
mod entities;
mod model;
mod sec;
mod setup;

use core_kit::rocket_users::{
  new_user,
  login,
  change_password,
  deauth_all_clients,
  remove_user,
};
use core_kit::rocket_phrases::{
  create_phrase,
};

use rocket::{get, launch, routes};

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
    .mount("/", routes![
      index,
      new_user,
      login,
      change_password,
      deauth_all_clients,
      remove_user,
      create_phrase,
    ])
}
