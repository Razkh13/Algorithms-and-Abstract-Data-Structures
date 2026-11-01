mod container;
mod stack;
mod array;
mod forward_list;
mod set;
mod hash_table;
mod lru;
mod tree;
mod dbms;

use std::env;
use dbms::DBMS;

fn main() {
    let args: Vec<String> = env::args().collect();
    let mut query = String::new();

    let mut i = 1;
    while i < args.len() {
        if args[i] == "--query" && i + 1 < args.len() { query = args[i + 1].clone(); i += 2; }
        else { i += 1; }
    }

    if query.is_empty() {
        println!("Usage: ./dbms --query '<CMD>'");
        return;
    }

    let mut db = DBMS::new();

    for one in query.split(';') {
        let trimmed = one.trim();
        if !trimmed.is_empty() { db.execute(trimmed); }
    }
}