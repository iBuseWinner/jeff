//! TBD.

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
