//! Core model.

pub mod serde_vec;
pub mod user;

pub type RedisPool = bb8_redis::bb8::Pool<bb8_redis::RedisConnectionManager>;
pub type PostgresPool = sea_orm::DatabaseConnection;
