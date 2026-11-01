use crate::container::Container;
use std::any::Any;

struct TNode {
    data: String,
    left: Option<Box<TNode>>,
    right: Option<Box<TNode>>,
    height: i32,
}

pub struct BST {
    root: Option<Box<TNode>>,
}

impl BST {
    pub fn new() -> Self { BST { root: None } }
    
    pub fn insert(&mut self, val: String) {
        fn ins(n: &mut Option<Box<TNode>>, v: String) {
            if n.is_none() { *n = Some(Box::new(TNode { data: v, left: None, right: None, height: 1 })); return; }
            let node = n.as_mut().unwrap();
            if v < node.data { ins(&mut node.left, v); }
            else if v > node.data { ins(&mut node.right, v); }
            
            let lh = node.left.as_ref().map_or(0, |x| x.height);
            let rh = node.right.as_ref().map_or(0, |x| x.height);
            node.height = 1 + std::cmp::max(lh, rh);
        }
        ins(&mut self.root, val);
    }
    pub fn height(&self) -> i32 { self.root.as_ref().map_or(0, |n| n.height) }
}

impl Container for BST {
    fn as_any(&self) -> &dyn Any { self }
    fn as_any_mut(&mut self) -> &mut dyn Any { self }
}