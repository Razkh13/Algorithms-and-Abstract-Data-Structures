#pragma once
#include "container.h"

class Stack : public Container {
    struct Node {
        std::string data;
        Node* next;
    };
    Node* top = nullptr;

    bool IsEmptyInternal() const { return top == nullptr; }
    std::string PeekInternal() const { return top ? top->data : ""; }
    void PopSilentInternal() {
        if (!top) return;
        Node* t = top;
        top = top->next;
        delete t;
    }

public:
    Stack() = default;
    ~Stack();

    void Push(const std::string& value) override;
    void Pop() override;
    void Print() const override;
    bool Find(const std::string& value) const override;

    void Save(std::ofstream& out) const override;
    void Load(std::ifstream& in) override;
    std::string TypeName() const override { return "STACK"; }

    bool ValidateXML(const std::string& s) const;
    std::string FixXML(const std::string& broken) const;
};