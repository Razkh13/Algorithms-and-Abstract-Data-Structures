use crate::container::Container;
use std::any::Any;

pub struct Stack {
    pub data: Vec<String>,
}

impl Stack {
    pub fn new() -> Self { Stack { data: Vec::new() } }
    pub fn push(&mut self, v: String) { self.data.push(v); }
    pub fn pop(&mut self) -> Option<String> { self.data.pop() }
    pub fn print(&self) { for x in self.data.iter().rev() { print!("{} ", x); } println!(); }
    pub fn peek(&self) -> Option<&String> { self.data.last() }
    pub fn is_empty(&self) -> bool { self.data.is_empty() }

    pub fn validate_xml(s: &str) -> bool {
        let mut st = Stack::new();
        let bytes = s.as_bytes();
        let mut i = 0;
        while i < bytes.len() {
            if bytes[i] != b'<' { return false; }
            let mut j = i + 1;
            while j < bytes.len() && bytes[j] != b'>' { j += 1; }
            if j == bytes.len() { return false; }
            let body = &s[i+1..j];
            
            if body.starts_with('/') {
                let name = &body[1..];
                if name.is_empty() || !name.chars().all(|c| c.is_ascii_lowercase()) { return false; }
                if st.is_empty() { return false; }
                if st.peek().unwrap() != name { return false; }
                st.pop();
            } else {
                if body.is_empty() || !body.chars().all(|c| c.is_ascii_lowercase()) { return false; }
                st.push(body.to_string());
            }
            i = j + 1;
        }
        st.is_empty()
    }

    pub fn fix_xml(broken: &str) -> String {
        if Stack::validate_xml(broken) { return broken.to_string(); }
        let alphabet = b"abcdefghijklmnopqrstuvwxyz<>/";
        let mut answer = String::new();
        let mut found = 0;
        let bytes = broken.as_bytes();

        for pos in 0..bytes.len() {
            for &c in alphabet {
                if c == bytes[pos] { continue; }
                let mut t = bytes.to_vec();
                t[pos] = c;
                let t_str = String::from_utf8(t).unwrap();
                if Stack::validate_xml(&t_str) {
                    if found == 0 { answer = t_str; }
                    found += 1;
                }
            }
        }
        if found > 0 { answer } else { "Incorrect".to_string() }
    }
}

impl Container for Stack {
    fn as_any(&self) -> &dyn Any { self }
    fn as_any_mut(&mut self) -> &mut dyn Any { self }
}