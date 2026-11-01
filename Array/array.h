#pragma once
#include "container.h"

class Array : public Container {
    std::string* data;
    int size;
    int capacity;

    void resize();

public:
    Array();
    ~Array();

    void Push(const std::string& value) override;
    void PushAt(int index, const std::string& value);
    void Pop() override;
    void PopAt(int index);
    void SetAt(int index, const std::string& value);
    std::string GetAt(int index);
    int Size() const;

    void Print() const override;
    bool Find(const std::string& value) const override;

    void Save(std::ofstream& out) const override;
    void Load(std::ifstream& in) override;
    std::string TypeName() const override { return "ARRAY"; }

    void PrintSubarrays();
};