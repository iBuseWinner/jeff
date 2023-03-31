//! Module to serialize `Vec<u8>` to `String` and back again.

use rocket::serde::{Serialize, Deserialize};
use rocket::serde::{Deserializer, Serializer};
use base64::{Engine as _, engine::general_purpose::STANDARD as base64};

pub fn serialize<S: Serializer>(v: &Vec<u8>, s: S) -> Result<S::Ok, S::Error> {
  let encs = base64.encode(v);
  String::serialize(&encs, s)
}

pub fn deserialize<'de, D: Deserializer<'de>>(d: D) -> Result<Vec<u8>, D::Error> {
  let decs = String::deserialize(d)?;
  base64.decode(decs.as_bytes()).map_err(|e| serde::de::Error::custom(e))
}
