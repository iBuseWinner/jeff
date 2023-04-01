//! Structs and functions needed for working with phrases routes.

use serde::{Deserialize, Serialize};
use serde_json::{Map, Value};

/// Struct for creating phrase request (see crate::core_kit::rocket_phrases::create_phrase).
#[derive(Deserialize, Serialize)]
pub struct CreatePhraseRequest {
  pub phrase: String,
  pub links: Vec<i64>,
  pub exec: bool,
  pub properties: Map<String, Value>,
}

/// Struct for getting phrase request (see crate::core_kit::rocket_phrases::get_phrase).
#[derive(Deserialize, Serialize)]
pub struct GetPhraseResponse {
  pub id: i64,
  pub phrase: String,
  pub links: Vec<i64>,
  pub exec: bool,
  pub properties: Map<String, Value>,
}

/// Links packer from `&Vec<i64>` to `String` (for db).
pub fn pack_links(links: &Vec<i64>) -> String {
  let mut packed = String::new();
  for link in links {
    packed.push_str(&link.to_string());
    packed.push(',');
  }
  packed.pop();
  packed
}

/// Links unpacker to `Vec<i64>` from `&String` (from db).
pub fn unpack_links(links: &String) -> Vec<i64> {
  links.split(',').map(|el| el.parse::<i64>()).try_collect::<Vec<i64>>().unwrap_or(Vec::new())
}
