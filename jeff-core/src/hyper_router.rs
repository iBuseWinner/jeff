use hyper::{Body, Method, http::{Request, Response}};
use std::{convert::Infallible, net::SocketAddr, sync::Arc};
use tokio::sync::Mutex;

use crate::model::{Workspace, Db, DaemonsHolder};
use crate::logic;

pub async fn router(
  req: Request<Body>, db: Db, admin_key: String, _addr: SocketAddr, _daemons: Arc<Mutex<DaemonsHolder>>
)
  -> Result<Response<Body>, Infallible>
{
  let ws = Workspace { req, db };
  Ok(match (ws.req.method(), ws.req.uri().path()) {
    // (&Method::OPTIONS, _) => options_answer(),
    (&Method::GET, "/favicon.ico") => logic::from_code_and_msg (404, None),
    (&Method::GET, "/db-setup")    => logic::db_setup          (ws, admin_key).await,
    (&Method::PUT, "/sign-up")     => logic::sign_up           (ws)           .await,
    (&Method::GET, "/sign-in")     => logic::sign_in           (ws)           .await,
    (method, path) => match logic::auth_by_token(&ws).await {
      Ok(user_id) => match (method, path) {
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
        _ => logic::from_code_and_msg(404, Some("Requested resourse doesn't exists.")),
      },
      Err((code, msg)) => logic::from_code_and_msg(code, Some(&msg)),
    },
  })
}
