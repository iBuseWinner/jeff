//! Security functions.

use sha3::{Digest, Sha3_256};

/// Hashes the pass.
pub fn hash_password(pass: &Vec<u8>) -> Vec<u8> {
  let mut hasher = Sha3_256::new();
  hasher.update(pass);
  hasher.finalize().to_vec()
}
