#include "array.h"
#include <iostream>
using namespace std;

Array::Array() {
    size = 0;
    capacity = 1;
    data = new string[capacity];
}

Array::~Array() {
    delete[] data;
}

void Array::resize() {
    int newCapacity = capacity * 2;
    if (newCapacity == 0) {
        newCapacity = 1;
    }

    string* temp = new string[newCapacity];
    for (int i = 0; i < size; i++) {
        temp[i] = data[i];
    }

    delete[] data;
    data = temp;
    capacity = newCapacity;
}

void Array::Push(const string& value) {
    if (size == capacity) {
        resize();
    }
    data[size] = value;
    size++;
}

void Array::PushAt(int index, const string& value) {
    if (index < 0 || index > size) {
        cout << "Некорректный индекс" << endl;
        return;
    }
    if (size == capacity) {
        resize();
    }
    for (int i = size; i > index; i--) {
        data[i] = data[i - 1];
    }
    data[index] = value;
    size++;
}

void Array::Pop() {
    if (size == 0) {
        cout << "Массив пуст" << endl;
        return;
    }
    cout << data[size - 1] << endl;
    size--;
}

void Array::PopAt(int index) {
    if (index < 0 || index >= size) {
        cout << "Некорректный индекс" << endl;
        return;
    }
    for (int i = index; i < size - 1; i++) {
        data[i] = data[i + 1];
    }
    size--;
}

void Array::SetAt(int index, const string& value) {
    if (index < 0 || index >= size) {
        cout << "Некорректный индекс" << endl;
        return;
    }
    data[index] = value;
}

string Array::GetAt(int index) {
    if (index < 0 || index >= size) {
        cout << "Некорректный индекс" << endl;
        return "";
    }
    return data[index];
}

int Array::Size() const {
    return size;
}

void Array::Print() const {
    for (int i = 0; i < size; i++) {
        cout << data[i] << " ";
    }
    cout << endl;
}

bool Array::Find(const string& value) const {
    for (int i = 0; i < size; i++) {
        if (data[i] == value) {
            return true;
        }
    }
    return false;
}

void Array::Save(ofstream& out) const {
    out << "Кол-во элементов: " << size << "\n";
    for (int i = 0; i < size; i++) {
        out << data[i] << " ";
    }
    out << "\n";
}

void Array::Load(ifstream& in) {
    string w1, w2;
    int n;
    if (!(in >> w1 >> w2 >> n) || n < 0) return;

    delete[] data;
    size = 0;
    if (n > 0) capacity = n;
    else capacity = 1;
    data = new string[capacity];

    for (int i = 0; i < n; i++) {
        if (!(in >> data[i])) break;
        size++;
    }
}

void Array::PrintSubarrays() {
    int n = size;
    cout << "[";

    bool first_global = true;

    for (int len = 0; len <= n; ++len) {
        for (int mask = 0; mask < (1 << n); ++mask) {
            
            int count = 0;
            for (int i = 0; i < n; ++i) {
                if (mask & (1 << i)) count++;
            }

            if (count == len) {
                if (!first_global) cout << ", ";
                first_global = false;

                cout << "{";
                bool first_inner = true;
                for (int i = 0; i < n; ++i) {
                    if (mask & (1 << i)) {
                        if (!first_inner) cout << ", ";
                        cout << data[i];
                        first_inner = false;
                    }
                }
                cout << "}";
            }
        }
    }
    cout << "]" << endl;
}