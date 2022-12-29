use std::{env, io, io::Read, process, fs, boxed::Box, net::SocketAddr};
use serde::{Deserialize, Serialize};

#[derive(Clone, Deserialize, Serialize)]
pub struct AppConfig {
  pub pg: String,
  pub admin_key: String,
  pub hyper_addr: SocketAddr,
}

impl AppConfig {
  pub fn load() -> AppConfig {
    match match env::args().nth(1) {
      None => AppConfig::stdin_setup(),
      Some(filepath) => AppConfig::parse_cfg_file(filepath),
    } {
      Ok(conf) => {
        println!("Configuration loaded.");
        conf
      },
      _ => {
        eprintln!("Configuration load error.");
        process::exit(1);
      },
    }
  }
  
  fn stdin_setup() -> Result<AppConfig, Box<dyn std::error::Error>> {
    let stdin = io::stdin();
    println!("Hello! This is Jeff server. Enter several params:");
    println!("PostgreSQL username:");
    let mut buffer = String::new();
    stdin.read_line(&mut buffer)?;
    let buffer = buffer.trim();
    let pg = String::from("host=localhost user='") + &buffer + &String::from("' password='");
    println!("PostgreSQL password:");
    let mut buffer = String::new();
    stdin.read_line(&mut buffer)?;
    let buffer = buffer.trim();
    let pg = pg + &buffer + &String::from("' connect_timeout=10 keepalives=0");
    println!("IP and port:");
    let mut buffer = String::new();
    stdin.read_line(&mut buffer)?;
    let buffer = buffer.trim();
    let hyper_addr: SocketAddr = buffer.parse()?;
    println!("Auth key (more than 64 symbols):");
    let mut buffer = String::new();
    stdin.read_line(&mut buffer)?;
    let admin_key = String::from(buffer.strip_suffix("\n").ok_or("")?);
    match admin_key.len() < 64 {
      true => Err(Box::new(io::Error::new(io::ErrorKind::Other, 
                                          "Key length is less than 64 symbols."))),
      false => Ok(AppConfig { pg, admin_key, hyper_addr }),
    }
  }
  
  fn parse_cfg_file(filepath: String) -> Result<AppConfig, Box<dyn std::error::Error>> {
    let mut file = fs::File::open(filepath)?;
    let mut buffer = String::new();
    file.read_to_string(&mut buffer)?;
    let conf: AppConfig = serde_json::from_str(&buffer)?;
    match conf.admin_key.len() < 64 {
      true => Err(Box::new(io::Error::new(io::ErrorKind::Other,
                                          "Key length is less than 64 symbols."))),
      false => Ok(conf),
    }
  }
}

pub fn get_config() -> AppConfig {
  AppConfig::load()
}
