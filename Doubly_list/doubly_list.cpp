#include "doubly_list.h"
#include <iostream>
using namespace std;

DoublyList::~DoublyList() {
    Node* cur = head;
    while (cur) {
        Node* t = cur;
        cur = cur->next;
        delete t;
    }
}

void DoublyList::Push(const string& value) {
    Node* n = new Node{value, head, nullptr};
    if (head) head->prev = n;
    else tail = n;
    head = n;
    size++;
}

void DoublyList::PushTail(const string& value) {
    Node* n = new Node{value, nullptr, tail};
    if (tail) tail->next = n;
    else head = n;
    tail = n;
    size++;
}

void DoublyList::PushBefore(const string& target, const string& value) {
    Node* cur = head;
    while (cur && cur->data != target) cur = cur->next;

    if (!cur) { cout << "Не найдено" << endl; return; }
    if (cur == head) { Push(value); return; }

    Node* n = new Node{value, cur, cur->prev};
    cur->prev->next = n;
    cur->prev = n;
    size++;
}

void DoublyList::PushAfter(const string& target, const string& value) {
    Node* cur = head;
    while (cur && cur->data != target) cur = cur->next;

    if (!cur) { cout << "Не найдено" << endl; return; }
    if (cur == tail) { PushTail(value); return; }

    Node* n = new Node{value, cur->next, cur};
    cur->next->prev = n;
    cur->next = n;
    size++;
}

void DoublyList::Pop() {
    if (!head) { cout << "Список пуст" << endl; return; }
    cout << head->data << endl;
    Node* t = head;
    head = head->next;
    if (head) head->prev = nullptr;
    else tail = nullptr;
    delete t;
    size--;
}

void DoublyList::PopTail() {
    if (!tail) { cout << "Список пуст" << endl; return; }
    cout << tail->data << endl;
    Node* t = tail;
    tail = tail->prev;
    if (tail) tail->next = nullptr;
    else head = nullptr;
    delete t;
    size--;
}

void DoublyList::PopBefore(const string& target) {
    Node* cur = head;
    while (cur && cur->data != target) cur = cur->next;

    if (!cur || !cur->prev) { cout << "Нечего удалять" << endl; return; }

    Node* t = cur->prev;
    cout << t->data << endl;

    if (t == head) { Pop(); return; }

    t->prev->next = cur;
    cur->prev = t->prev;
    delete t;
    size--;
}

void DoublyList::PopAfter(const string& target) {
    Node* cur = head;
    while (cur && cur->data != target) cur = cur->next;

    if (!cur || !cur->next) { cout << "Нечего удалять" << endl; return; }

    Node* t = cur->next;
    cout << t->data << endl;

    if (t == tail) { PopTail(); return; }

    cur->next = t->next;
    t->next->prev = cur;
    delete t;
    size--;
}

void DoublyList::PopValue(const string& value) {
    Node* cur = head;
    while (cur && cur->data != value) cur = cur->next;

    if (!cur) { cout << "Не найдено" << endl; return; }

    if (cur == head) { Pop(); return; }
    if (cur == tail) { PopTail(); return; }

    cout << cur->data << endl;
    cur->prev->next = cur->next;
    cur->next->prev = cur->prev;
    delete cur;
    size--;
}

bool DoublyList::Find(const string& value) const {
    Node* cur = head;
    while (cur) {
        if (cur->data == value) return true;
        cur = cur->next;
    }
    return false;
}

void DoublyList::Print() const {
    Node* cur = head;
    while (cur) {
        cout << cur->data << " ";
        cur = cur->next;
    }
    cout << endl;
}

void DoublyList::PrintReverse() const {
    Node* cur = tail;
    while (cur) {
        cout << cur->data << " ";
        cur = cur->prev;
    }
    cout << endl;
}

void DoublyList::PrintEverySecond() const {
    Node* cur = head;
    int i = 0;
    while (cur) {
        if (i % 2 == 0) cout << cur->data << " ";
        cur = cur->next;
        i++;
    }
    cout << endl;
}

void DoublyList::Save(ofstream& out) const {
    out << "Кол-во элементов: " << size << "\n";
    Node* cur = head;
    while (cur) {
        out << cur->data << " ";
        cur = cur->next;
    }
    out << "\n";
}

void DoublyList::Load(ifstream& in) {
    while (head) {
        Node* t = head;
        head = head->next;
        delete t;
    }
    head = nullptr;
    tail = nullptr;
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