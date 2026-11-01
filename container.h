#pragma once
#include <string>
#include <fstream>

class Container {
public:
    virtual ~Container() = default;

    virtual void Push(const std::string& value) = 0;
    virtual void Pop() = 0;
    virtual void Print() const = 0;
    virtual bool Find(const std::string& value) const = 0;

    virtual void Save(std::ofstream& out) const = 0;
    virtual void Load(std::ifstream& in) = 0;

    virtual std::string TypeName() const = 0;
};