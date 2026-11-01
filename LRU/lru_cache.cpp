#include "lru_cache.h"
#include <iostream>
using namespace std;

LRUCache::LRUCache(int cap) {
    capacity = cap;
    size = 0;
    head = new Node("", "");
    tail = new Node("", "");
    head->next = tail;
    tail->prev = head;
    cache = new HashTable(10007, 1);
}

LRUCache::~LRUCache() {
    Node* cur = head;
    while (cur) {
        Node* next = cur->next;
        delete cur;
        cur = next;
    }
    delete cache;
}

void LRUCache::addToHead(Node* node) {
    node->prev = head;
    node->next = head->next;
    head->next->prev = node;
    head->next = node;
}

void LRUCache::removeNode(Node* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

void LRUCache::moveToHead(Node* node) {
    removeNode(node);
    addToHead(node);
}

void LRUCache::popTail() {
    Node* res = tail->prev;
    if (res != head) {
        removeNode(res);
        cache->Remove(res->key);
        delete res;
        size--;
    }
}

void LRUCache::Set(const string& key, const string& value) {
    Node* cur = head->next;
    while (cur != tail) {
        if (cur->key == key) {
            cur->value = value;
            moveToHead(cur);
            return;
        }
        cur = cur->next;
    }

    if (size == capacity) {
        popTail();
    }
    Node* newNode = new Node(key, value);
    addToHead(newNode);
    cache->Add(key);
    size++;
}

string LRUCache::Get(const string& key) {
    Node* cur = head->next;
    while (cur != tail) {
        if (cur->key == key) {
            moveToHead(cur);
            return cur->value;
        }
        cur = cur->next;
    }
    return "-1";
}

void LRUCache::Save(ofstream& out) const {
    out << "Кол-во элементов: " << size << "\n";
    Node* cur = head->next;
    while (cur != tail) {
        out << cur->key << " " << cur->value << " ";
        cur = cur->next;
    }
    out << "\n";
}

void LRUCache::Load(ifstream& in, int cap) {
    capacity = cap;
    Node* cur = head->next;
    while (cur != tail) {
        Node* next = cur->next;
        delete cur;
        cur = next;
    }
    head->next = tail;
    tail->prev = head;
    size = 0;

    string w1, w2;
    int n;
    if (!(in >> w1 >> w2 >> n) || n < 0) return;
    for (int i = 0; i < n; i++) {
        string k, v;
        if (!(in >> k >> v)) break;
        Set(k, v);
    }
}