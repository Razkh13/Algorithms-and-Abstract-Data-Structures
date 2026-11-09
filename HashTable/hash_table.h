#pragma once
#include "container.h"
#include <string>

class HashTable : public Container {
    struct Entry {
        std::string key;
        std::string value;
        bool is_deleted;
        Entry() : key(""), value(""), is_deleted(false) {}
    };

    int capacity;
    int size;
    int method; 
    Entry* table; 

    int hash1(const std::string& key) const;
    int hash2(const std::string& key) const;

public:
    HashTable(int cap = 10007, int m = 1);
    ~HashTable() override;

    void Add(const std::string& key, const std::string& value);
    bool Contains(const std::string& key) const;
    std::string Get(const std::string& key) const;
    void Remove(const std::string& key);

    int LengthOfLongestSubstring(const std::string& s);

    void SetMethod(int m) { method = m; }
    int GetMethod() const { return method; }

    void Push(const std::string& value) override { Add(value, ""); }
    void Pop() override;
    void Print() const override;
    bool Find(const std::string& value) const override { return Contains(value); }

    void Save(std::ofstream& out) const override;
    void Load(std::ifstream& in) override;
    std::string TypeName() const override { return "HASHTABLE"; }
};