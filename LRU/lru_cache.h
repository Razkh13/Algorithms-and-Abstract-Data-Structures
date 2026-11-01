#pragma once
#include <string>
#include <fstream>
#include "HashTable/hash_table.h"

class LRUCache {
    struct Node {
        std::string key;
        std::string value;
        Node* prev;
        Node* next;
        Node(std::string k, std::string v) : key(k), value(v), prev(nullptr), next(nullptr) {}
    };

    int capacity;
    int size;
    Node* head;
    Node* tail;
    HashTable* cache;

    void addToHead(Node* node);
    void removeNode(Node* node);
    void moveToHead(Node* node);
    void popTail();

public:
    LRUCache(int cap);
    ~LRUCache();

    void Set(const std::string& key, const std::string& value);
    std::string Get(const std::string& key);

    int GetCapacity() const { return capacity; }
    void Save(std::ofstream& out) const;
    void Load(std::ifstream& in, int cap);
};