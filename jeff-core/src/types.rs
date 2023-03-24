use serde::{Deserialize, Serialize};

#[derive(Deserialize, Serialize)]
pub struct SignUpRequestData {
  pub login: String,
  pub name: String,
  #[serde(with="base64")]
  pub phash: Vec<u8>,
}

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

mod base64 {
  use rocket::serde::{Serialize, Deserialize};
  use rocket::serde::{Deserializer, Serializer};
  use base64::{Engine as _, engine::general_purpose};

  pub fn serialize<S: Serializer>(v: &Vec<u8>, s: S) -> Result<S::Ok, S::Error> {
    let base64 = general_purpose::STANDARD.encode(v);
    String::serialize(&base64, s)
  }
  
  pub fn deserialize<'de, D: Deserializer<'de>>(d: D) -> Result<Vec<u8>, D::Error> {
    let base64 = String::deserialize(d)?;
    general_purpose::STANDARD.decode(base64.as_bytes()).map_err(|e| serde::de::Error::custom(e))
  }
}
