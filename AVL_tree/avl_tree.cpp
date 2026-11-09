#include "avl_tree.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <sstream>
#include <cmath>
using namespace std;

AVLTree::~AVLTree() {
    Clear(root);
}

int AVLTree::Height(Node* n) const {
    if (n == nullptr) return 0;
    return n->height;
}

void AVLTree::UpdateHeight(Node* n) {
    if (n == nullptr) return;
    int lh = Height(n->left);
    int rh = Height(n->right);
    n->height = 1 + (lh > rh ? lh : rh);
}

AVLTree::Node* AVLTree::Insert(Node* n, const string& value) {
    if (n == nullptr) return new Node(value);

    if (value < n->data) {
        n->left = Insert(n->left, value);
        if (n->left != nullptr) n->left->parent = n;
    }
    else if (value > n->data) {
        n->right = Insert(n->right, value);
        if (n->right != nullptr) n->right->parent = n;
    }
    else {
        return n;
    }

    UpdateHeight(n);
    return n; 
}

AVLTree::Node* AVLTree::Minimum(Node* n) const {
    if (n == nullptr) return nullptr;
    while (n->left != nullptr) n = n->left;
    return n;
}

AVLTree::Node* AVLTree::Remove(Node* n, const string& value) {
    if (n == nullptr) return nullptr;

    if (value < n->data) {
        n->left = Remove(n->left, value);
        if (n->left != nullptr) n->left->parent = n;
    }
    else if (value > n->data) {
        n->right = Remove(n->right, value);
        if (n->right != nullptr) n->right->parent = n;
    }
    else {
        if (n->left == nullptr || n->right == nullptr) {
            Node* temp = n->left ? n->left : n->right;
            if (temp == nullptr) {
                delete n;
                return nullptr;
            } else {
                temp->parent = n->parent;
                delete n;
                return temp;
            }
        }
        else {
            Node* temp = Minimum(n->right);
            n->data = temp->data;
            n->right = Remove(n->right, temp->data);
            if (n->right != nullptr) n->right->parent = n;
        }
    }

    UpdateHeight(n);
    return n;
}

void AVLTree::Push(const string& value) {
    if (Find(value)) return;
    root = Insert(root, value);
    if (root != nullptr) root->parent = nullptr;
}

void AVLTree::Pop() {
    if (root == nullptr) {
        cout << "Дерево пусто" << endl;
        return;
    }
    PopValue(root->data);
}

void AVLTree::PopValue(const string& value) {
    if (!Find(value)) {
        cout << "Не найдено" << endl;
        return;
    }
    cout << value << endl;
    root = Remove(root, value);
    if (root != nullptr) root->parent = nullptr;
}

bool AVLTree::Find(const string& value) const {
    Node* n = root;
    while (n != nullptr) {
        if (n->data == value) return true;
        if (value < n->data) n = n->left;
        else n = n->right;
    }
    return false;
}

void AVLTree::Print() const {
    PrintBFS();
}

void AVLTree::PrintBFS() const {
    if (root == nullptr) {
        cout << "Дерево пусто" << endl;
        return;
    }

    vector<Node*> q;
    q.push_back(root);
    size_t i = 0;
    while (i < q.size()) {
        Node* node = q[i++];
        cout << node->data << " ";
        if (node->left) q.push_back(node->left);
        if (node->right) q.push_back(node->right);
    }
    cout << endl;
}

void AVLTree::Clear(Node* n) {
    if (n == nullptr) return;
    Clear(n->left);
    Clear(n->right);
    delete n;
}

int AVLTree::CountNodes(Node* n) const {
    if (n == nullptr) return 0;
    return 1 + CountNodes(n->left) + CountNodes(n->right);
}

void AVLTree::SaveNode(Node* n, ofstream& out) const {
    if (n == nullptr) return;
    out << n->data << " ";
    SaveNode(n->left, out);
    SaveNode(n->right, out);
}

void AVLTree::Save(ofstream& out) const {
    out << "Кол-во элементов: " << CountNodes(root) << "\n";
    SaveNode(root, out);
    out << "\n";
}

void AVLTree::Load(ifstream& in) {
    Clear(root);
    root = nullptr;

    string w1, w2;
    int n;
    if (!(in >> w1 >> w2 >> n) || n < 0) return;

    for (int i = 0; i < n; i++) {
        string v;
        if (!(in >> v)) break;
        Push(v);
    }
}

struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;
    
    HuffmanNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        if (l->freq != r->freq) {
            return l->freq > r->freq; 
        }
        return l->ch > r->ch;
    }
};

static void BuildCodes(HuffmanNode* root, string str, map<char, string>& huffmanCode) {
    if (!root) return;
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }
    BuildCodes(root->left, str + "0", huffmanCode);
    BuildCodes(root->right, str + "1", huffmanCode);
}

static void FreeTree(HuffmanNode* root) {
    if (!root) return;
    FreeTree(root->left);
    FreeTree(root->right);
    delete root;
}

void AVLTree::HuffmanEncode(const string& text) {
    if (text.empty()) {
        cout << "Пустая строка" << endl;
        return;
    }

    map<char, int> freq;
    for (char c : text) freq[c]++;

    if (freq.size() == 1) {
        char ch = freq.begin()->first;
        cout << "Таблица кодов:" << endl;
        cout << ch << " 0" << endl;
        cout << "Закодированная строка:" << endl;
        for (size_t i = 0; i < text.length(); ++i) {
            cout << "0";
        }
        cout << endl;
        return;
    }

    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;
    for (auto pair : freq) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    while (pq.size() != 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();

        HuffmanNode* top = new HuffmanNode('\0', left->freq + right->freq);
        top->left = left;
        top->right = right;
        pq.push(top);
    }

    HuffmanNode* root = pq.top();
    map<char, string> huffmanCode;
    BuildCodes(root, "", huffmanCode);

    cout << "Таблица кодов:" << endl;
    for (auto pair : huffmanCode) {
        cout << pair.first << " " << pair.second << endl;
    }

    string encoded = "";
    for (char c : text) {
        encoded += huffmanCode[c];
    }
    
    cout << "Закодированная строка:" << endl;
    cout << encoded << endl;

    FreeTree(root);
}

void AVLTree::HuffmanDecode(const string& code, const string& table_str) {
    if (code.empty() || table_str.empty()) {
        cout << "Пустой код или таблица" << endl;
        return;
    }

    map<char, string> huffmanCode;
    stringstream ss(table_str);
    string item;
    while (getline(ss, item, ',')) {
        if (item.size() >= 3) {
            char ch = item[0];
            string c = item.substr(2);
            huffmanCode[ch] = c;
        }
    }

    HuffmanNode* root = new HuffmanNode('\0', 0);
    for (auto pair : huffmanCode) {
        HuffmanNode* cur = root;
        for (char bit : pair.second) {
            if (bit == '0') {
                if (!cur->left) cur->left = new HuffmanNode('\0', 0);
                cur = cur->left;
            } else {
                if (!cur->right) cur->right = new HuffmanNode('\0', 0);
                cur = cur->right;
            }
        }
        cur->ch = pair.first;
    }

    string decoded = "";
    HuffmanNode* cur = root;
    for (char bit : code) {
        if (bit == '0') {
            if (!cur->left) { cout << "Ошибка декодирования" << endl; FreeTree(root); return; }
            cur = cur->left;
        } else {
            if (!cur->right) { cout << "Ошибка декодирования" << endl; FreeTree(root); return; }
            cur = cur->right;
        }

        if (!cur->left && !cur->right) {
            decoded += cur->ch;
            cur = root;
        }
    }
    cout << decoded << endl;
    FreeTree(root);
}