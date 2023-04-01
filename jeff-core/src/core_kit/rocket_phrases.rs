//! Rocket routes to work with phrases.

use crate::entities::{prelude::*, *};
use crate::model::{DbPool, MResult, RedisPool, phrase::*};
use crate::sec::*;

use rocket::{State, serde::json::Json, http::CookieJar};
use rocket::{get, put, patch, delete};
use sea_orm::*;
use serde_json::{Value as JsonValue, Map as JsonMap};

/// Checks if the phrase exists and belongs to the user.
async fn check_phrase(user_id: &i64, phrase_id: &i64, db: &State<DbPool>) -> MResult<phrases::Model> {
  let phrase: phrases::Model = Phrases::find_by_id(*phrase_id)
    .one(db as &DbPool)
    .await?
    .ok_or::<String>(format!("There is no phrase by id {}.", *phrase_id))?;
  if phrase.user_id == *user_id { return Ok(phrase) }
  else { return Err("Phrase access is denied.".into()) }
}

/// Creates a new phrase.
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
    user_id: ActiveValue::set(user_id),
    phrase: ActiveValue::set(data.phrase.clone()),
    links: ActiveValue::set(Some(pack_links(&data.links))),
    exec: ActiveValue::set(data.exec),
    properties: ActiveValue::set(serde_json::to_string(&data.properties)?),
    ..Default::default()
  };
  let res = Phrases::insert(new_phrase).exec(db as &DbPool).await?;
  Ok(res.last_insert_id.to_string())
}

/// Gets the phrase from database.
#[get("/phrase/<phrase_id>")]
pub async fn get_phrase(
  phrase_id: i64,
  db: &State<DbPool>,
  cacher: &State<RedisPool>,
  jar: &CookieJar<'_>,
) -> MResult<String> {
  let user_id = check_user(jar, cacher).await?;
  let phrase = check_phrase(&user_id, &phrase_id, db).await?;
  let links: String = phrase.links.unwrap_or(String::new());
  let links: Vec<i64> = unpack_links(&links);
  let phrase = GetPhraseResponse {
    id: phrase.id,
    phrase: phrase.phrase,
    links,
    exec: phrase.exec,
    properties: serde_json::from_str(&phrase.properties)?,
  };
  Ok(serde_json::to_string(&phrase)?)
}

/// Patches the phrase.
#[patch("/phrase/<phrase_id>", data = "<data>")]
pub async fn patch_phrase(
  phrase_id: i64,
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
  let phrase = check_phrase(&user_id, &phrase_id, db).await?;
  let mut phrase: phrases::ActiveModel = phrase.into();
  if let Some(phrase_text) = generic_patch.get("phrase") {
    phrase.phrase = ActiveValue::set(phrase_text.to_string());
  }
  if let Some(links) = generic_patch.get("links") {
    let links = links.to_string();
    if links.is_empty() {
      phrase.links = ActiveValue::set(None);
    } else {
      phrase.links = ActiveValue::set(Some(links));
    }
  }
  if let Some(exec) = generic_patch.get("exec") {
    phrase.exec = ActiveValue::set(exec.as_bool().ok_or::<String>("`exec` must be a boolean.".into())?);
  }
  if let Some(properties) = generic_patch.get("properties") {
    phrase.properties = ActiveValue::set(
      serde_json::to_string(
        properties.as_object().ok_or::<String>("`properties` must be an object.".into())?
      )?
    );
  }
  phrase.update(db as &DbPool).await?;
  Ok(())
}

/// Removes the phrase.
#[delete("/phrase/<phrase_id>")]
pub async fn remove_phrase(
  phrase_id: i64,
  db: &State<DbPool>,
  cacher: &State<RedisPool>,
  jar: &CookieJar<'_>,
) -> MResult<()> {
  let user_id = check_user(jar, cacher).await?;
  let phrase = check_phrase(&user_id, &phrase_id, db).await?;
  phrase.delete(db as &DbPool).await?;
  Ok(())
}
