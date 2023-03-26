//! TBD.

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
