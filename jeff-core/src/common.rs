use sea_orm::DbErr;
use rocket::response::Responder;
use std::{io, fmt};
use serde_json::Error as JsonError;

#[derive(Responder, Debug)]
#[response(status = 500, content_type = "json")]
pub struct ErrorResponder {
  message: String,
}

impl fmt::Display for ErrorResponder {
  fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
    write!(f, "{}", self.message)
  }
}

impl From<DbErr> for ErrorResponder {
  fn from(err: DbErr) -> ErrorResponder {
    ErrorResponder { message: err.to_string() }
  }
}

impl From<String> for ErrorResponder {
  fn from(string: String) -> ErrorResponder {
    ErrorResponder { message: string }
  }
}

impl From<&str> for ErrorResponder {
  fn from(str: &str) -> ErrorResponder {
    str.to_owned().into()
  }
}

impl From<io::Error> for ErrorResponder {
  fn from(err: io::Error) -> ErrorResponder {
    err.to_string().into()
  }
}

impl From<JsonError> for ErrorResponder {
  fn from(err: JsonError) -> ErrorResponder {
    err.to_string().into()
  }
}
