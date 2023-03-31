//! Core model.

pub mod error_handler;
pub mod phrase;
pub mod serde_vec;
pub mod user;

pub type RedisPool = bb8_redis::bb8::Pool<bb8_redis::RedisConnectionManager>;
pub type DbPool = sea_orm::DatabaseConnection;

pub use error_handler::ErrorResponder;

pub type MResult<T> = Result<T, ErrorResponder>;
