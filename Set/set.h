#pragma once
#include "container.h"
#include "Forward_list/forward_list.h"
#include <vector>

class Set : public Container {
    static const int TABLE_SIZE = 100003;
    ForwardList** buckets;

    size_t hash(const std::string& s) const;

public:
    Set();
    ~Set() override;

    void Add(const std::string& value);
    void Del(const std::string& value);
    bool Contains(const std::string& value) const;

    void Push(const std::string& value) override;
    void Pop() override;
    void Print() const override;
    bool Find(const std::string& value) const override;

    void Save(std::ofstream& out) const override;
    void Load(std::ifstream& in) override;
    std::string TypeName() const override { return "SET"; }

    std::vector<std::string> GetAllElements() const;
    void Partition(int k);
};