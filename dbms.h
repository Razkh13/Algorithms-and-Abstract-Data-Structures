#pragma once
#include <map>
#include <string>
#include "container.h"

class HashTable;
class LRUCache;

class DBMS {
    std::map<std::string, Container*> containers;
    std::map<std::string, HashTable*> hashTables;
    std::map<std::string, LRUCache*> lruCaches;
    std::string dataFile;

public:
    DBMS(const std::string& file) : dataFile(file) {}
    ~DBMS();

    void LoadAll();
    void SaveAll();
    void ExecuteQuery(const std::string& query);

private:
    Container* GetOrCreate(const std::string& name, const std::string& type);
    Container* CreateByPrefix(char prefix);
};