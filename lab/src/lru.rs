use std::collections::HashMap;

struct LRUNode {
    key: String,
    value: String,
    prev: *mut LRUNode,
    next: *mut LRUNode,
}

pub struct LRUCache {
    capacity: usize,
    size: usize,
    head: *mut LRUNode,
    tail: *mut LRUNode,
    map: HashMap<String, *mut LRUNode>,
}

impl LRUCache {
    pub fn new(cap: usize) -> Self {
        let head = Box::into_raw(Box::new(LRUNode { key: String::new(), value: String::new(), prev: std::ptr::null_mut(), next: std::ptr::null_mut() }));
        let tail = Box::into_raw(Box::new(LRUNode { key: String::new(), value: String::new(), prev: std::ptr::null_mut(), next: std::ptr::null_mut() }));
        unsafe { (*head).next = tail; (*tail).prev = head; }
        LRUCache { capacity: cap, size: 0, head, tail, map: HashMap::new() }
    }
    fn add_to_head(&mut self, node: *mut LRUNode) {
        unsafe {
            (*node).prev = self.head;
            (*node).next = (*self.head).next;
            (*(*self.head).next).prev = node;
            (*self.head).next = node;
        }
    }
    fn remove_node(&mut self, node: *mut LRUNode) {
        unsafe {
            (*(*node).prev).next = (*node).next;
            (*(*node).next).prev = (*node).prev;
        }
    }
    fn move_to_head(&mut self, node: *mut LRUNode) {
        self.remove_node(node);
        self.add_to_head(node);
    }
    pub fn set(&mut self, key: String, value: String) {
        if let Some(&node) = self.map.get(&key) {
            unsafe { (*node).value = value; }
            self.move_to_head(node);
            return;
        }
        if self.size == self.capacity {
            unsafe {
                let res = (*self.tail).prev;
                self.remove_node(res);
                self.map.remove(&(*res).key);
                drop(Box::from_raw(res));
            }
            self.size -= 1;
        }
        let new_node = Box::into_raw(Box::new(LRUNode { key: key.clone(), value, prev: std::ptr::null_mut(), next: std::ptr::null_mut() }));
        self.add_to_head(new_node);
        self.map.insert(key, new_node);
        self.size += 1;
    }
    pub fn get(&mut self, key: &str) -> String {
        if let Some(&node) = self.map.get(key) {
            self.move_to_head(node);
            unsafe { return (*node).value.clone(); }
        }
        "-1".to_string()
    }
}

impl Drop for LRUCache {
    fn drop(&mut self) {
        let mut cur = self.head;
        while !cur.is_null() {
            unsafe {
                let next = (*cur).next;
                drop(Box::from_raw(cur));
                cur = next;
            }
        }
    }
}