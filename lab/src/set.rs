#[allow(dead_code)]
use crate::container::Container;
use crate::forward_list::ForwardList;
use std::any::Any;

pub struct Set {
    buckets: Vec<ForwardList>,
    table_size: usize,
}

impl Set {
    pub fn new() -> Self {
        let mut buckets = Vec::with_capacity(10007);
        for _ in 0..10007 { buckets.push(ForwardList::new()); }
        Set { buckets, table_size: 10007 }
    }

    fn hash(&self, s: &str) -> usize {
        let mut h: usize = 0;
        for c in s.bytes() { h = h.wrapping_mul(31).wrapping_add(c as usize); }
        h % self.table_size
    }

    pub fn add(&mut self, v: String) {
        let idx = self.hash(&v);
        if !self.buckets[idx].find(&v) {
            self.buckets[idx].push_tail(v);
        }
    }

    pub fn del(&mut self, v: &str) {
        let idx = self.hash(v);
        if self.buckets[idx].find(v) {
            self.buckets[idx].pop_value(v);
        }
    }

    pub fn contains(&self, v: &str) -> bool {
        let idx = self.hash(v);
        self.buckets[idx].find(v)
    }

    pub fn print(&self) {
        for b in &self.buckets {
            for x in b.get_all() {
                print!("{} ", x);
            }
        }
        println!();
    }

    pub fn get_all_elements(&self) -> Vec<String> {
        let mut res = Vec::new();
        for b in &self.buckets {
            res.extend(b.get_all());
        }
        res
    }

    pub fn partition(&self, k: usize) {
        let raw = self.get_all_elements();
        let mut nums: Vec<i32> = raw.iter().filter_map(|s| s.parse().ok()).collect();
        let total_sum: i32 = nums.iter().sum();

        if k == 0 || total_sum % (k as i32) != 0 {
            println!("Невозможно разбить: сумма не делится на {}", k);
            return;
        }
        let target = total_sum / (k as i32);
        nums.sort_by(|a, b| b.cmp(a));

        let mut groups = vec![-1; nums.len()];
        let mut current_sums = vec![0; k];

        fn backtrack(idx: usize, nums: &[i32], target: i32, k: usize, current_sums: &mut Vec<i32>, groups: &mut Vec<i32>) -> bool {
            if idx == nums.len() { return true; }
            for i in 0..k {
                if current_sums[i] + nums[idx] <= target {
                    if i > 0 && current_sums[i] == 0 && current_sums[i - 1] == 0 { continue; }
                    current_sums[i] += nums[idx];
                    groups[idx] = i as i32;
                    if backtrack(idx + 1, nums, target, k, current_sums, groups) { return true; }
                    current_sums[i] -= nums[idx];
                    groups[idx] = -1;
                }
            }
            false
        }

        if backtrack(0, &nums, target, k, &mut current_sums, &mut groups) {
            println!("Разбиение на {} множеств (сумма {}):", k, target);
            for i in 0..k {
                print!("{{ ");
                for j in 0..nums.len() {
                    if groups[j] == i as i32 {
                        print!("{} ", nums[j]);
                    }
                }
                println!("}}");
            }
        } else {
            println!("Невозможно разбить на {} равных подмножеств.", k);
        }
    }
}

impl Container for Set {
    fn as_any(&self) -> &dyn Any { self }
    fn as_any_mut(&mut self) -> &mut dyn Any { self }
}