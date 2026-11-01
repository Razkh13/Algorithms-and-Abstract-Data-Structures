#include "stack.h"
#include <iostream>
using namespace std;

Stack::~Stack() {
    while (top) {
        Node* t = top;
        top = top->next;
        delete t;
    }
}

void Stack::Push(const string& value) {
    top = new Node{value, top};
}

void Stack::Pop() {
    if (!top) {
        cout << "Стек пуст" << endl;
        return;
    }
    cout << top->data << endl;
    Node* t = top;
    top = top->next;
    delete t;
}

void Stack::Print() const {
    Node* cur = top;
    while (cur) {
        cout << cur->data << " ";
        cur = cur->next;
    }
    cout << endl;
}

bool Stack::Find(const string& value) const {
    Node* cur = top;
    while (cur) {
        if (cur->data == value) return true;
        cur = cur->next;
    }
    return false;
}

void Stack::Save(ofstream& out) const {
    int n = 0;
    Node* cur = top;
    while (cur) { n++; cur = cur->next; }

    out << "Кол-во элементов: " << n << "\n";

    Stack temp;
    cur = top;
    while (cur) {
        temp.Push(cur->data);
        cur = cur->next;
    }

    while (!temp.IsEmptyInternal()) {
        out << temp.PeekInternal() << " ";
        temp.PopSilentInternal();
    }
    out << "\n";
}

void Stack::Load(ifstream& in) {
    while (top) {
        Node* t = top;
        top = top->next;
        delete t;
    }

    string w1, w2;
    int n;
    if (!(in >> w1 >> w2 >> n) || n < 0) return;

    Stack temp;
    for (int i = 0; i < n; i++) {
        string v;
        if (!(in >> v)) break;
        temp.Push(v);
    }

    while (!temp.IsEmptyInternal()) {
        Push(temp.PeekInternal());
        temp.PopSilentInternal();
    }
}

bool Stack::ValidateXML(const std::string& s) const {
    Stack tmp;
    size_t i = 0;
    while (i < s.size()) {
        if (s[i] != '<') return false;
        size_t j = s.find('>', i);
        if (j == std::string::npos) return false;

        std::string body = s.substr(i + 1, j - i - 1);

        if (!body.empty() && body[0] == '/') {
            std::string name = body.substr(1);
            if (name.empty()) return false;
            for (char c : name) if (!(c >= 'a' && c <= 'z')) return false;
            if (tmp.IsEmptyInternal()) return false;
            if (tmp.PeekInternal() != name) return false;
            tmp.PopSilentInternal();
        } else {
            if (body.empty()) return false;
            for (char c : body) if (!(c >= 'a' && c <= 'z')) return false;
            tmp.Push(body);
        }
        i = j + 1;
    }
    return tmp.IsEmptyInternal();
}

string Stack::FixXML(const std::string& broken) const {
    if (ValidateXML(broken)) return broken;

    const std::string alphabet = "abcdefghijklmnopqrstuvwxyz<>/";
    std::string answer = "";
    int found = 0;

    for (size_t pos = 0; pos < broken.size(); ++pos) {
        for (char c : alphabet) {
            if (c == broken[pos]) continue;
            std::string t = broken;
            t[pos] = c;
            if (ValidateXML(t)) {
                if (found == 0) answer = t;
                ++found;
            }
        }
    }
    
    if (found > 0) return answer;
    return "Incorrect";
}