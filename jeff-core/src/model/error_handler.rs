//! Some common things to all JC code.

use base64::DecodeError;
use bb8_redis::redis::RedisError;
use bb8_redis::bb8::RunError;
use sea_orm::DbErr;
use rocket::response::Responder;
use std::{io, fmt, num::ParseIntError};
use serde_json::Error as JsonError;

/// Custom error struct for Rocket and other functions.
#[derive(Responder, Debug)]
#[response(status = 400, content_type = "json")]
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

impl From<ParseIntError> for ErrorResponder {
  fn from(err: ParseIntError) -> ErrorResponder {
    err.to_string().into()
  }
}

impl From<RedisError> for ErrorResponder {
  fn from(err: RedisError) -> ErrorResponder {
    err.to_string().into()
  }
}

impl From<RunError<RedisError>> for ErrorResponder {  
  fn from(err: RunError<RedisError>) -> ErrorResponder {
    err.to_string().into()
  }
}

impl From<DecodeError> for ErrorResponder {
  fn from(err: DecodeError) -> ErrorResponder {
    err.to_string().into()
  }
}
