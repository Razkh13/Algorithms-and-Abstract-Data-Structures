#pragma once
#include "container.h"

class AVLTree : public Container {
    struct Node {
        std::string data;
        int height;
        Node *left, *right, *parent;

        Node(const std::string& value) {
            data = value;
            height = 1;
            left = nullptr;
            right = nullptr;
            parent = nullptr;
        }
    };

    Node* root = nullptr;

    int Height(Node* n) const;
    void UpdateHeight(Node* n);
    Node* Insert(Node* n, const std::string& value);
    Node* Remove(Node* n, const std::string& value);
    Node* Minimum(Node* n) const;

    void Clear(Node* n);
    int CountNodes(Node* n) const;
    void SaveNode(Node* n, std::ofstream& out) const;

public:
    AVLTree() = default;
    ~AVLTree();

    void Push(const std::string& value) override;
    void Pop() override;
    void PopValue(const std::string& value);
    void Print() const override;
    bool Find(const std::string& value) const override;

    void PrintBFS() const;

    void Save(std::ofstream& out) const override;
    void Load(std::ifstream& in) override;
    std::string TypeName() const override { return "AVLTREE"; }

    int GetHeight() const;
    void HuffmanEncode(const std::string& text);
    void HuffmanDecode(const std::string& code, const std::string& table_str);
};