//! Structs needed for working with users routes.

use crate::model::serde_vec;

use serde::{Deserialize, Serialize};

/// Struct for sign up request (see at `crate::core_kit::users::new_user`).
#[derive(Deserialize, Serialize)]
pub struct SignUpRequestData {
  pub login: String,
  pub name: String,
  #[serde(with="serde_vec")]
  pub phash: Vec<u8>,
}

/// Struct for sign in request (see at `crate::core_kit::users::login`).
#[derive(Deserialize, Serialize)]
pub struct SignInRequestData {
  pub login: String,
  #[serde(with="serde_vec")]
  pub phash: Vec<u8>,
}

/// Struct for changing password request (see at `crate::core_kit::users::change_password`).
#[derive(Deserialize, Serialize)]
pub struct ChangePasswordRequestData {
  #[serde(with="serde_vec")]
  pub old_phash: Vec<u8>,
  #[serde(with="serde_vec")]
  pub new_phash: Vec<u8>,
}

/// Struct for storing user settings.
#[derive(Deserialize, Serialize)]
pub struct UserSettings {
  pub name: String,
  pub save_history_opt: bool,
  pub monologue_mode_opt: bool,
  pub scenario_exit_opt: String,
  pub repeated_explicit_in_db_opt: bool,
}

impl Default for UserSettings {
  fn default() -> UserSettings {
    UserSettings {
      name: "".into(),
      save_history_opt: true,
      monologue_mode_opt: false,
      scenario_exit_opt: "//e".into(),
      repeated_explicit_in_db_opt: true,
    }
  }
}
