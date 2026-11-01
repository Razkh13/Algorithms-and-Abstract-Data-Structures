#pragma once
#include "container.h"
#include <vector>

class ForwardList : public Container {
    struct Node {
        std::string data;
        Node* next;
    };
    Node* head = nullptr;
    int size = 0;

public:
    ForwardList() = default;
    ~ForwardList();

    void Push(const std::string& value) override;
    void PushTail(const std::string& value);
    void PushBefore(const std::string& target, const std::string& value);
    void PushAfter(const std::string& target, const std::string& value);

    void Pop() override;
    void PopTail();
    void PopBefore(const std::string& target);
    void PopAfter(const std::string& target);
    void PopValue(const std::string& value);

    bool Find(const std::string& value) const override;
    void Print() const override;
    void PrintReverse() const;
    void PrintEverySecond() const;

    void Save(std::ofstream& out) const override;
    void Load(std::ifstream& in) override;
    std::string TypeName() const override { return "FORWARD_LIST"; }

    bool IsEmpty() const;
    std::vector<std::string> GetAll() const;
    void Clear();
};