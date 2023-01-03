use std::{env, io, io::Read, process, fs, boxed::Box, net::SocketAddr};
use serde::{Deserialize, Serialize};

use crate::model::{AppConfig, Daemon, DaemonsHolder};

// Configuration loader.

impl AppConfig {
  pub fn load() -> AppConfig {
    match match env::args().nth(1) {
      None => AppConfig::no_args(),
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
  
  fn no_args() -> Result<AppConfig, Box<dyn std::error::Error>> {
    Err(Box::new(io::Error::new(io::ErrorKind::Other, "No args received.")))
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

// Daemons' starter.

impl DaemonsHolder {
  pub fn load(commands: &Vec<Daemon>) -> DaemonsHolder {
    let mut daemons = vec![];
    for daemon in commands {
      if daemon.cmd.len() < 1 { continue; }
      let cmd_args: Vec<&str> = daemon.cmd.split(' ').collect();
      let proc = match cmd_args.len() > 1 {
        true => tokio::process::Command::new(cmd_args[0])
          .args(&cmd_args[1..])
          .kill_on_drop(true)
          .spawn(),
        false => tokio::process::Command::new(cmd_args[0])
          .kill_on_drop(true)
          .spawn(),
      };
      daemons.push((daemon.to_owned(), proc));
    }
    DaemonsHolder { spawned: daemons }
  }
}
