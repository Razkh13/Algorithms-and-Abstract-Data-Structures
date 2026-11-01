use std::collections::HashMap;
use crate::container::Container;
use crate::stack::Stack;
use crate::array::Array;
use crate::set::Set;
use crate::hash_table::HashTable;
use crate::lru::LRUCache;
use crate::tree::BST;

pub struct DBMS {
    pub containers: HashMap<String, Box<dyn Container>>,
    pub hash_tables: HashMap<String, HashTable>,
    pub lru_caches: HashMap<String, LRUCache>,
}

impl DBMS {
    pub fn new() -> Self {
        DBMS { containers: HashMap::new(), hash_tables: HashMap::new(), lru_caches: HashMap::new() }
    }

    pub fn execute(&mut self, query: &str) {
        let parts: Vec<&str> = query.split_whitespace().collect();
        if parts.is_empty() { return; }
        let cmd = parts[0];

        if cmd == "HTADD" && parts.len() >= 4 {
            let name = parts[1];
            let method = parts[2].parse().unwrap_or(1);
            let val = parts[3].to_string();
            let ht = self.hash_tables.entry(name.to_string()).or_insert_with(|| HashTable::new(10007, method));
            ht.add(val);
            ht.print();
            return;
        }
        if cmd == "HTDEL" && parts.len() >= 3 {
            let name = parts[1];
            let val = parts[2];
            if let Some(ht) = self.hash_tables.get_mut(name) {
                ht.remove(val);
                ht.print();
            }
            return;
        }
        if cmd == "HTGET" && parts.len() >= 3 {
            let name = parts[1];
            let val = parts[2];
            if let Some(ht) = self.hash_tables.get(name) {
                if ht.contains(val) { println!("TRUE"); }
                else { println!("FALSE"); }
            } else {
                println!("FALSE");
            }
            return;
        }
        if cmd == "HTLONGEST" && parts.len() >= 2 {
            let mut ht = HashTable::new(10007, 1);
            println!("{}", ht.length_of_longest_substring(parts[1]));
            return;
        }

        if cmd == "LRUSET" && parts.len() >= 5 {
            let name = parts[1];
            let cap = parts[2].parse().unwrap_or(2);
            let key = parts[3].to_string();
            let val = parts[4].to_string();
            let lru = self.lru_caches.entry(name.to_string()).or_insert_with(|| LRUCache::new(cap));
            lru.set(key, val);
            return;
        }
        if cmd == "LRUGET" && parts.len() >= 3 {
            let name = parts[1];
            let key = parts[2];
            if let Some(lru) = self.lru_caches.get_mut(name) { println!("{}", lru.get(key)); }
            else { println!("-1"); }
            return;
        }

        if cmd == "SVALIDATE" && parts.len() >= 3 {
            println!("{}", if Stack::validate_xml(parts[2]) { "TRUE" } else { "FALSE" });
            return;
        }
        if cmd == "SFIXXML" && parts.len() >= 3 {
            println!("{}", Stack::fix_xml(parts[2]));
            return;
        }

        if cmd == "THUFFMAN" && parts.len() >= 2 {
            let text = parts[1];
            let mut freq = HashMap::new();
            for c in text.chars() { *freq.entry(c).or_insert(0) += 1; }
            let mut sorted: Vec<_> = freq.into_iter().collect();
            sorted.sort_by(|a, b| b.1.cmp(&a.1));
            
            println!("Таблица кодов:");
            let mut code_map = HashMap::new();
            for (i, (c, _)) in sorted.iter().enumerate() {
                let code = format!("{:b}", i + 1);
                println!("{} {}", c, code);
                code_map.insert(*c, code);
            }
            let mut encoded = String::new();
            for c in text.chars() { encoded.push_str(code_map.get(&c).unwrap()); }
            println!("Закодированная строка:");
            println!("{}", encoded);
            return;
        }

        let name = if parts.len() > 1 { parts[1] } else { "" };
        let prefix = cmd.chars().next().unwrap_or(' ');

        if prefix == 'S' && cmd == "SPUSH" && parts.len() >= 3 {
            let c = self.containers.entry(name.to_string()).or_insert_with(|| Box::new(Stack::new()));
            if let Some(s) = c.as_any_mut().downcast_mut::<Stack>() { s.push(parts[2].to_string()); s.print(); }
        }
        else if cmd == "SPOP" && parts.len() >= 2 {
            if let Some(c) = self.containers.get_mut(name) {
                if let Some(s) = c.as_any_mut().downcast_mut::<Stack>() {
                    if let Some(v) = s.pop() { println!("{}", v); }
                    s.print();
                }
            }
        }
        else if prefix == 'M' && cmd == "MPUSH" && parts.len() >= 3 {
            let c = self.containers.entry(name.to_string()).or_insert_with(|| Box::new(Array::new()));
            if let Some(a) = c.as_any_mut().downcast_mut::<Array>() { a.push(parts[2].to_string()); a.print(); }
        }
        else if cmd == "MSUBARRAYS" && parts.len() >= 2 {
            if let Some(c) = self.containers.get(name) {
                if let Some(a) = c.as_any().downcast_ref::<Array>() { a.print_subarrays(); }
            }
        }
        else if cmd == "SETADD" && parts.len() >= 3 {
            let c = self.containers.entry(name.to_string()).or_insert_with(|| Box::new(Set::new()));
            if let Some(s) = c.as_any_mut().downcast_mut::<Set>() { s.add(parts[2].to_string()); s.print(); }
        }
        else if cmd == "SET_AT" && parts.len() >= 3 {
            if let Some(c) = self.containers.get(name) {
                if let Some(s) = c.as_any().downcast_ref::<Set>() {
                    if s.contains(parts[2]) { println!("TRUE"); }
                    else { println!("FALSE"); }
                }
            }
        }
        else if cmd == "SETPARTITION" && parts.len() >= 3 {
            let k = parts[2].parse().unwrap_or(1);
            if let Some(c) = self.containers.get(name) {
                if let Some(s) = c.as_any().downcast_ref::<Set>() { s.partition(k); }
            }
        }
        else if cmd == "TINSERT" && parts.len() >= 3 {
            let c = self.containers.entry(name.to_string()).or_insert_with(|| Box::new(BST::new()));
            if let Some(t) = c.as_any_mut().downcast_mut::<BST>() { t.insert(parts[2].to_string()); }
        }
        else if cmd == "THEIGHT" && parts.len() >= 2 {
            if let Some(c) = self.containers.get(name) {
                if let Some(t) = c.as_any().downcast_ref::<BST>() { println!("{}", t.height()); }
            }
        }
        else {
            println!("Неизвестная команда");
        }
    }
}