//! TBD.

use serde::{Deserialize, Serialize};
use serde_json::{Map, Value};

/// TBD.
#[derive(Deserialize, Serialize)]
pub struct CreatePhraseRequest {
  pub phrase: String,
  pub links: Vec<i64>,
  pub exec: bool,
  pub properties: Map<String, Value>,
}

/// TBD.
#[derive(Deserialize, Serialize)]
pub struct GetPhraseResponse {
  pub id: i64,
  pub phrase: String,
  pub links: Vec<i64>,
  pub exec: bool,
  pub properties: Map<String, Value>,
}

/// TBD.
pub fn pack_links(links: &Vec<i64>) -> String {
  let mut packed = String::new();
  for link in links {
    packed.push_str(&link.to_string());
    packed.push(',');
  }
  packed.pop();
  packed
}

/// TBD.
pub fn unpack_links(links: &String) -> Vec<i64> {
  links.split(',').map(|el| el.parse::<i64>()).try_collect::<Vec<i64>>().unwrap_or(Vec::new())
}
