#include "forward_list.h"
#include <iostream>
using namespace std;

ForwardList::~ForwardList() {
    while (head) {
        Node* t = head;
        head = head->next;
        delete t;
    }
}

void ForwardList::Push(const string& value) {
    head = new Node{value, head};
    size++;
}

void ForwardList::PushTail(const string& value) {
    Node* n = new Node{value, nullptr};
    if (!head) {
        head = n;
    } else {
        Node* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = n;
    }
    size++;
}

void ForwardList::PushBefore(const string& target, const string& value) {
    if (!head) { cout << "Список пуст" << endl; return; }
    if (head->data == target) { Push(value); return; }

    Node* cur = head;
    while (cur->next && cur->next->data != target)
        cur = cur->next;

    if (!cur->next) { cout << "Не найдено" << endl; return; }

    cur->next = new Node{value, cur->next};
    size++;
}

void ForwardList::PushAfter(const string& target, const string& value) {
    Node* cur = head;
    while (cur && cur->data != target) cur = cur->next;

    if (!cur) { cout << "Не найдено" << endl; return; }

    cur->next = new Node{value, cur->next};
    size++;
}

void ForwardList::Pop() {
    if (!head) { cout << "Список пуст" << endl; return; }
    cout << head->data << endl;
    Node* t = head;
    head = head->next;
    delete t;
    size--;
}

void ForwardList::PopTail() {
    if (!head) { cout << "Список пуст" << endl; return; }
    if (!head->next) {
        cout << head->data << endl;
        delete head;
        head = nullptr;
        size--;
        return;
    }

    Node* cur = head;
    while (cur->next->next) cur = cur->next;

    cout << cur->next->data << endl;
    delete cur->next;
    cur->next = nullptr;
    size--;
}

void ForwardList::PopBefore(const string& target) {
    if (!head || !head->next) { cout << "Нечего удалять" << endl; return; }
    if (head->next->data == target) { Pop(); return; }

    Node* cur = head;
    while (cur->next && cur->next->next && cur->next->next->data != target)
        cur = cur->next;

    if (!cur->next || !cur->next->next) { cout << "Не найдено" << endl; return; }

    Node* t = cur->next;
    cout << t->data << endl;
    cur->next = t->next;
    delete t;
    size--;
}

void ForwardList::PopAfter(const string& target) {
    Node* cur = head;
    while (cur && cur->data != target) cur = cur->next;

    if (!cur || !cur->next) { cout << "Нечего удалять" << endl; return; }

    Node* t = cur->next;
    cout << t->data << endl;
    cur->next = t->next;
    delete t;
    size--;
}

void ForwardList::PopValue(const string& value) {
    if (!head) { cout << "Список пуст" << endl; return; }
    if (head->data == value) { Pop(); return; }

    Node* cur = head;
    while (cur->next && cur->next->data != value)
        cur = cur->next;

    if (!cur->next) { cout << "Не найдено" << endl; return; }

    Node* t = cur->next;
    cout << t->data << endl;
    cur->next = t->next;
    delete t;
    size--;
}

bool ForwardList::Find(const string& value) const {
    Node* cur = head;
    while (cur) {
        if (cur->data == value) return true;
        cur = cur->next;
    }
    return false;
}

void ForwardList::Print() const {
    Node* cur = head;
    while (cur) {
        cout << cur->data << " ";
        cur = cur->next;
    }
    cout << endl;
}

void ForwardList::PrintReverse() const {
    if (!head) { cout << endl; return; }
    string* buf = new string[size];
    Node* cur = head;
    for (int i = 0; i < size; i++) {
        buf[i] = cur->data;
        cur = cur->next;
    }
    for (int i = size - 1; i >= 0; i--)
        cout << buf[i] << " ";
    cout << endl;
    delete[] buf;
}

void ForwardList::PrintEverySecond() const {
    Node* cur = head;
    int i = 0;
    while (cur) {
        if (i % 2 == 0) cout << cur->data << " ";
        cur = cur->next;
        i++;
    }
    cout << endl;
}

void ForwardList::Save(ofstream& out) const {
    out << "Кол-во элементов: " << size << "\n";
    Node* cur = head;
    while (cur) {
        out << cur->data << " ";
        cur = cur->next;
    }
    out << "\n";
}

void ForwardList::Load(ifstream& in) {
    while (head) {
        Node* t = head;
        head = head->next;
        delete t;
    }
    size = 0;

    string w1, w2;
    int n;
    if (!(in >> w1 >> w2 >> n) || n < 0) return;

    for (int i = 0; i < n; i++) {
        string v;
        if (!(in >> v)) break;
        PushTail(v);
    }
}

bool ForwardList::IsEmpty() const {
    return head == nullptr;
}

std::vector<std::string> ForwardList::GetAll() const {
    std::vector<std::string> res;
    Node* cur = head;
    while (cur) {
        res.push_back(cur->data);
        cur = cur->next;
    }
    return res;
}

void ForwardList::Clear() {
    while (head) {
        Node* t = head;
        head = head->next;
        delete t;
    }
    size = 0;
}