#include "hash_table.h"
#include <iostream>
#include <fstream>
using namespace std;

HashTable::HashTable(int cap, int m) {
    capacity = cap;
    size = 0;
    method = m;
    table = new Entry[capacity];
}

HashTable::~HashTable() {
    delete[] table; 
}

int HashTable::hash1(const string& key) const {
    int h = 0;
    for (char c : key) h = (h * 31 + c) % capacity;
    return h;
}

int HashTable::hash2(const string& key) const {
    int h = 0;
    for (char c : key) h = (h * 37 + c);
    int step = (capacity - 1) - (h % (capacity - 1));
    return step == 0 ? 1 : step;
}

void HashTable::Add(const string& key) {
    if (size >= capacity) return;

    int idx = hash1(key);
    int step = (method == 2) ? hash2(key) : 1;
    int start_idx = idx;

    while (table[idx].key != "" && !table[idx].is_deleted) {
        if (table[idx].key == key) return;
        idx = (idx + step) % capacity;
        if (idx == start_idx) return;
    }

    table[idx].key = key;
    table[idx].is_deleted = false;
    size++;
}

bool HashTable::Contains(const string& key) const {
    int idx = hash1(key);
    int step = (method == 2) ? hash2(key) : 1;
    int start_idx = idx;

    while (table[idx].key != "" || table[idx].is_deleted) {
        if (!table[idx].is_deleted && table[idx].key == key) return true;
        idx = (idx + step) % capacity;
        if (idx == start_idx) break;
    }
    return false;
}

void HashTable::Remove(const string& key) {
    int idx = hash1(key);
    int step = (method == 2) ? hash2(key) : 1;
    int start_idx = idx;

    while (table[idx].key != "" || table[idx].is_deleted) {
        if (!table[idx].is_deleted && table[idx].key == key) {
            table[idx].is_deleted = true;
            table[idx].key = "";
            size--;
            return;
        }
        idx = (idx + step) % capacity;
        if (idx == start_idx) break;
    }
}

void HashTable::Pop() {
    for (int i = 0; i < capacity; ++i) {
        if (table[i].key != "" && !table[i].is_deleted) {
            cout << table[i].key << endl;
            table[i].is_deleted = true;
            table[i].key = "";
            size--;
            return;
        }
    }
    cout << "Таблица пуста" << endl;
}

void HashTable::Print() const {
    for (int i = 0; i < capacity; ++i) {
        if (table[i].key != "" && !table[i].is_deleted) {
            cout << table[i].key << " ";
        }
    }
    cout << endl;
}

void HashTable::Save(ofstream& out) const {
    out << "Кол-во элементов: " << size << "\n";
    for (int i = 0; i < capacity; ++i) {
        if (table[i].key != "" && !table[i].is_deleted) {
            out << table[i].key << " ";
        }
    }
    out << "\n";
}

void HashTable::Load(ifstream& in) {
    for (int i = 0; i < capacity; ++i) {
        table[i].key = "";
        table[i].is_deleted = false;
    }
    size = 0;
    
    string w1, w2;
    int n;
    if (!(in >> w1 >> w2 >> n) || n < 0) return;
    for (int i = 0; i < n; i++) {
        string v;
        if (!(in >> v)) break;
        Add(v);
    }
}

int HashTable::LengthOfLongestSubstring(const string& s) {
    if (s.empty()) return 0;
    for (int i = 0; i < capacity; ++i) {
        table[i].key = "";
        table[i].is_deleted = false;
    }
    size = 0;

    int max_len = 0;
    int left = 0;

    for (int right = 0; right < (int)s.size(); ++right) {
        string char_str(1, s[right]);
        
        while (Contains(char_str)) {
            string left_char(1, s[left]);
            Remove(left_char);
            left++;
        }
        
        Add(char_str);
        if (right - left + 1 > max_len) {
            max_len = right - left + 1;
        }
    }
    
    return max_len;
}