//! TBD.

use crate::entities::{prelude::*, *};
use crate::model::{DbPool, MResult, RedisPool, phrase::*};
use crate::sec::*;

use rocket::{State, serde::json::Json, http::CookieJar};
use rocket::{put, patch};
use sea_orm::*;
use serde_json::{Value as JsonValue, Map as JsonMap};

/// TBD.
async fn check_phrase(user_id: &i64, phrase_id: &i64, db: &State<DbPool>) -> MResult<()> {
  let phrase: Option<phrases::Model> = Phrases::find_by_id(*phrase_id)
    .one(db as &DbPool)
    .await?;
  let phrase_user_id: i64 = phrase
    .ok_or::<String>(format!("There is no phrase by id {}.", *phrase_id))?
    .user_id;
  if phrase_user_id == *user_id { return Ok(()) }
  else { return Err("Phrase access is denied.".into()) }
}

/// TBD.
#[put("/phrase", format = "json", data = "<data>")]
pub async fn create_phrase(
  data: Json<CreatePhraseRequest>,
  db: &State<DbPool>,
  cacher: &State<RedisPool>,
  jar: &CookieJar<'_>,
) -> MResult<String> {
  let user_id = check_user(jar, cacher).await?;
  for phrase_id in &data.links { check_phrase(&user_id, &phrase_id, db).await?; }
  let new_phrase = phrases::ActiveModel {
    phrase: ActiveValue::set(data.phrase.clone()),
    links: ActiveValue::set(Some(pack_links(&data.links))),
    exec: ActiveValue::set(data.exec),
    properties: ActiveValue::set(serde_json::to_string(&data.properties)?),
    ..Default::default()
  };
  let res = Phrases::insert(new_phrase).exec(db as &DbPool).await?;
  Ok(res.last_insert_id.to_string())
}

/// TBD.
#[patch("/phrase", data = "<data>")]
pub async fn patch_phrase(
  data: String,
  db: &State<DbPool>,
  cacher: &State<RedisPool>,
  jar: &CookieJar<'_>,
) -> MResult<()> {
  let user_id = check_user(jar, cacher).await?;
  let generic_patch: JsonValue = serde_json::from_str(&data)?;
  let generic_patch: &JsonMap<String, JsonValue> = generic_patch
    .as_object()
    .ok_or::<String>("Required data type is JSON.".into())?;
  if !generic_patch.contains_key("phrase_id") { return Err("Patch doesn't contain phrase id.".into()) }
  unimplemented!();
}
