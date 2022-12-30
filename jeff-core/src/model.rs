//! Модель данных приложения.

use chrono::{DateTime, Utc, serde::ts_seconds};
use custom_error::custom_error;
use hyper::{Body, body::to_bytes, http::Request};
use serde::{Deserialize, Serialize, de::DeserializeOwned};

use crate::psql::Db;
use crate::sec::auth::UserCredentials;

// custom_error!{ pub GetMutCardError{} = "Не удалось получить мутабельную карточку." }

pub struct Workspace {
  pub req: Request<Body>,
  pub db: Db,
}

#[derive(Deserialize, Serialize)]
pub struct User {
  pub id: i64,
  pub shared_boards: Vec<i64>,
  pub user_creds: UserCredentials,
}

custom_error!{ pub ExtractionError
  FromBody = "Не удалось получить данные из тела запроса.",
  FromBytes = "Не удалось создать строку из набора байт тела запроса.",
  FromBase64 = "Не удалось декодировать данные из base64.",
  FromJson = "Не удалось десериализовать JSON."
}

pub async fn extract<T>(req: Request<Body>) -> Result<T, ExtractionError>
  where
    T: DeserializeOwned,
{
  let body = match to_bytes(req.into_body()).await {
    Err(_) => return Err(ExtractionError::FromBody),
    Ok(v) => v,
  };
  let body = match String::from_utf8(body.to_vec()) {
    Err(_) => return Err(ExtractionError::FromBytes),
    Ok(v) => v.clone(),
  };
  let body = match base64::decode(&body) {
    Err(_) => return Err(ExtractionError::FromBase64),
    Ok(v) => match String::from_utf8(v) {
      Err(_) => return Err(ExtractionError::FromBase64),
      Ok(v) => v,
    },
  };
  match serde_json::from_str::<T>(&body) {
    Err(_) => Err(ExtractionError::FromJson),
    Ok(v) => Ok(v),
  }
}
