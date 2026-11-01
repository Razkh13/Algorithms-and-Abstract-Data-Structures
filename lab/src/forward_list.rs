#![allow(dead_code)]

use crate::container::Container;
use std::any::Any;

struct FNode { data: String, next: Option<Box<FNode>> }

pub struct ForwardList {
    head: Option<Box<FNode>>,
    pub size: usize,
}

impl ForwardList {
    pub fn new() -> Self { ForwardList { head: None, size: 0 } }
    pub fn push_tail(&mut self, v: String) {
        let n = Box::new(FNode { data: v, next: None });
        if self.head.is_none() { self.head = Some(n); }
        else {
            let mut cur = self.head.as_mut().unwrap();
            while cur.next.is_some() { cur = cur.next.as_mut().unwrap(); }
            cur.next = Some(n);
        }
        self.size += 1;
    }
    pub fn find(&self, v: &str) -> bool {
        let mut cur = self.head.as_ref();
        while let Some(n) = cur { if n.data == v { return true; } cur = n.next.as_ref(); }
        false
    }
    pub fn pop_value(&mut self, v: &str) -> Option<String> {
        if self.head.is_none() { return None; }
        if self.head.as_ref().unwrap().data == v {
            let old = self.head.take().unwrap();
            self.head = old.next; self.size -= 1; return Some(old.data);
        }
        let mut cur = self.head.as_mut().unwrap();
        while let Some(next) = cur.next.as_mut() {
            if next.data == v {
                let r = cur.next.take().unwrap();
                cur.next = r.next; self.size -= 1; return Some(r.data);
            }
            cur = cur.next.as_mut().unwrap();
        }
        None
    }
    pub fn get_all(&self) -> Vec<String> {
        let mut res = Vec::new();
        let mut cur = self.head.as_ref();
        while let Some(n) = cur { res.push(n.data.clone()); cur = n.next.as_ref(); }
        res
    }
}

impl Container for ForwardList {
    fn as_any(&self) -> &dyn Any { self }
    fn as_any_mut(&mut self) -> &mut dyn Any { self }
}