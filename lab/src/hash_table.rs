use crate::container::Container;
use std::any::Any;

pub struct HashTable {
    table: Vec<Option<String>>,
    capacity: usize,
    size: usize,
    method: i32,
}

impl HashTable {
    pub fn new(cap: usize, m: i32) -> Self {
        HashTable { table: vec![None; cap], capacity: cap, size: 0, method: m }
    }
    fn hash1(&self, key: &str) -> usize {
        let mut h: usize = 0;
        for c in key.bytes() { h = h.wrapping_mul(31).wrapping_add(c as usize); }
        h % self.capacity
    }
    fn hash2(&self, key: &str) -> usize {
        let mut h: usize = 0;
        for c in key.bytes() { h = h.wrapping_mul(37).wrapping_add(c as usize); }
        let step = (self.capacity - 1) - (h % (self.capacity - 1));
        if step == 0 { 1 } else { step }
    }
    pub fn add(&mut self, key: String) {
        if self.size >= self.capacity { return; }
        let mut idx = self.hash1(&key);
        let step = if self.method == 2 { self.hash2(&key) } else { 1 };
        let start_idx = idx;
        while let Some(ref k) = self.table[idx] {
            if k == &key { return; }
            idx = (idx + step) % self.capacity;
            if idx == start_idx { return; }
        }
        self.table[idx] = Some(key);
        self.size += 1;
    }
    pub fn contains(&self, key: &str) -> bool {
        let mut idx = self.hash1(key);
        let step = if self.method == 2 { self.hash2(key) } else { 1 };
        let start_idx = idx;
        while let Some(ref k) = self.table[idx] {
            if k == key { return true; }
            idx = (idx + step) % self.capacity;
            if idx == start_idx { break; }
        }
        false
    }
    pub fn remove(&mut self, key: &str) {
        let mut idx = self.hash1(key);
        let step = if self.method == 2 { self.hash2(key) } else { 1 };
        let start_idx = idx;
        while let Some(ref k) = self.table[idx] {
            if k == key { self.table[idx] = None; self.size -= 1; return; }
            idx = (idx + step) % self.capacity;
            if idx == start_idx { break; }
        }
    }
        pub fn print(&self) {
        for k in &self.table { if let Some(s) = k { print!("{} ", s); } }
        println!();
    }
    pub fn length_of_longest_substring(&mut self, s: &str) -> usize {
        self.table = vec![None; self.capacity]; self.size = 0;
        let mut max_len = 0;
        let mut left = 0;
        let chars: Vec<char> = s.chars().collect();
        for right in 0..chars.len() {
            let c_str = chars[right].to_string();
            while self.contains(&c_str) {
                let l_str = chars[left].to_string();
                self.remove(&l_str);
                left += 1;
            }
            self.add(c_str);
            if right - left + 1 > max_len { max_len = right - left + 1; }
        }
        max_len
    }
}

impl Container for HashTable {
    fn as_any(&self) -> &dyn Any { self }
    fn as_any_mut(&mut self) -> &mut dyn Any { self }
}