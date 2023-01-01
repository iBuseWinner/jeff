use hyper::{Body, Method, http::{Request, Response}};
use std::{convert::Infallible, net::SocketAddr};

use crate::model::Workspace;
use crate::psql::Db;

pub fn from_code_and_msg(code: u16, msg: Option<&str>) -> Response<Body> {
  Response::builder()
    .header("Content-Type", "text/html; charset=utf-8")
    //.header("Access-Control-Allow-Origin", "http://localhost:3000")
    //.header("Access-Control-Allow-Credentials", "true")
    .status(code)
    .body(match msg {
      None => Body::empty(),
      Some(msg) => Body::from(String::from(msg)),
    })
    .unwrap()
}

// pub fn options_answer() -> Response<Body> {
//   Response::builder()
//     .header("Access-Control-Allow-Origin", "http://localhost:3000")
//     .header("Access-Control-Allow-Credentials", "true")
//     .header("Access-Control-Allow-Methods", "GET, POST, PUT, PATCH, DELETE, OPTIONS")
//     .header("Access-Control-Allow-Headers", "App-Token")
//     .body(Body::empty())
//     .unwrap()
// }

pub async fn router(req: Request<Body>, db: Db, admin_key: String, _addr: SocketAddr)
  -> Result<Response<Body>, Infallible>
{
  let ws = Workspace { req, db };
  Ok(match (ws.req.method(), ws.req.uri().path()) {
    // (&Method::OPTIONS, _) => options_answer(),
    (&Method::GET, "/favicon.ico") => from_code_and_msg        (404, None),
    (&Method::GET, "/db-setup")    => logic::db_setup          (ws, admin_key).await,
    (&Method::PUT, "/sign-up")     => logic::sign_up           (ws)           .await,
    (&Method::GET, "/sign-in")     => logic::sign_in           (ws)           .await,
    (method, path) => match logic::auth_by_token(&ws).await {
      Ok((user_id, billed)) => match (method, path) {
        (&Method::DELETE, "/user")        => logic::delete_account    (ws, user_id)  .await,
        (&Method::PATCH,  "/user/creds")  => logic::patch_user_creds  (ws, user_id)  .await,
        (&Method::PUT,    "/source")      => logic::new_source        (ws, user_id)  .await,
        (&Method::PUT,    "/import")      => logic::import_to_source  (ws, user_id)  .await,
        (&Method::GET,    "/export")      => logic::export_source     (ws, user_id)  .await,
        (&Method::PUT,    "/source/expr") => logic::new_expression    (ws, user_id)  .await,
        (&Method::PATCH,  "/source/expr") => logic::patch_expression  (ws, user_id)  .await,
        (&Method::DELETE, "/source/expr") => logic::delete_expression (ws, user_id)  .await,
        (&Method::GET,    "/answer")      => logic::get_answer        (ws, user_id)  .await,
        (&Method::GET,    "/memory-cell") => logic::get_memory_cell   (ws, user_id)  .await,
        (&Method::PUT,    "/memory-cell") => logic::put_memory_cell   (ws, user_id)  .await,
        (&Method::GET,    "/history")     => logic::get_history       (ws, user_id)  .await,
        (&Method::DELETE, "/history")     => logic::clear_history     (ws, user_id)  .await,
        _ => from_code_and_msg(404, Some("Requested resourse doesn't exists.")),
      },
      Err((code, msg)) => from_code_and_msg(code, Some(&msg)),
    },
  })
}
