#include "set.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
using namespace std;

Set::Set() {
    buckets = new ForwardList*[TABLE_SIZE];
    for (int i = 0; i < TABLE_SIZE; ++i) {
        buckets[i] = new ForwardList();
    }
}

Set::~Set() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        delete buckets[i];
    }
    delete[] buckets;
}

size_t Set::hash(const string& s) const {
    size_t h = 0;
    for (char c : s) h = h * 31 + c;
    return h % TABLE_SIZE;
}

void Set::Add(const string& value) {
    size_t idx = hash(value);
    if (!buckets[idx]->Find(value)) {
        buckets[idx]->PushTail(value);
    }
}

void Set::Del(const string& value) {
    size_t idx = hash(value);
    if (buckets[idx]->Find(value)) {
        buckets[idx]->PopValue(value);
    }
}

bool Set::Contains(const string& value) const {
    size_t idx = hash(value);
    return buckets[idx]->Find(value);
}

void Set::Push(const string& value) { Add(value); }

void Set::Pop() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        if (!buckets[i]->IsEmpty()) {
            vector<string> items = buckets[i]->GetAll();
            cout << items[0] << endl;
            buckets[i]->PopValue(items[0]);
            return;
        }
    }
    cout << "Множество пусто" << endl;
}

void Set::Print() const {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        if (!buckets[i]->IsEmpty()) {
            vector<string> items = buckets[i]->GetAll();
            for (const auto& s : items) cout << s << " ";
        }
    }
    cout << endl;
}

bool Set::Find(const string& value) const { return Contains(value); }

void Set::Save(ofstream& out) const {
    int count = 0;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        count += buckets[i]->GetAll().size();
    }
    out << "Кол-во элементов: " << count << "\n";
    for (int i = 0; i < TABLE_SIZE; ++i) {
        for (const auto& s : buckets[i]->GetAll()) out << s << " ";
    }
    out << "\n";
}

void Set::Load(ifstream& in) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        buckets[i]->Clear();
    }
    string w1, w2;
    int n;
    if (!(in >> w1 >> w2 >> n) || n < 0) return;
    for (int i = 0; i < n; i++) {
        string v;
        if (!(in >> v)) break;
        Add(v);
    }
}

vector<string> Set::GetAllElements() const {
    vector<string> res;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        if (!buckets[i]->IsEmpty()) {
            vector<string> items = buckets[i]->GetAll();
            for (const auto& s : items) res.push_back(s);
        }
    }
    return res;
}

void Set::Partition(int target_sum) {
    vector<int> nums;
    vector<string> raw = GetAllElements();
    int total_sum = 0;

    for (const string& s : raw) {
        try {
            int val = stoi(s);
            nums.push_back(val);
            total_sum += val;
        } catch (const std::invalid_argument&) {
            cout << "Ошибка: элемент '" << s << "' не является числом!" << endl;
            return;
        } catch (const std::out_of_range&) {
            cout << "Ошибка: число '" << s << "' слишком большое!" << endl;
            return;
        }
    }

    if (target_sum <= 0 || total_sum % target_sum != 0) {
        cout << "Невозможно разбить: сумма " << total_sum << " не делится на " << target_sum << endl;
        return;
    }

    int k = total_sum / target_sum; 
    sort(nums.rbegin(), nums.rend());

    if (!nums.empty() && nums[0] > target_sum) {
        cout << "Невозможно разбить: есть число больше целевой суммы " << target_sum << endl;
        return;
    }

    vector<int> groups(nums.size(), -1);
    vector<int> current_sums(k, 0);

    function<bool(int)> backtrack = [&](int index) -> bool {
        if (index == (int)nums.size()) {
            return true;
        }

        for (int i = 0; i < k; ++i) {
            if (current_sums[i] + nums[index] <= target_sum) {
                if (i > 0 && current_sums[i] == 0 && current_sums[i - 1] == 0) continue;

                current_sums[i] += nums[index];
                groups[index] = i;

                if (backtrack(index + 1)) return true;

                current_sums[i] -= nums[index];
                groups[index] = -1;
            }
        }
        return false;
    };

    if (backtrack(0)) {
        cout << "Разбиение на " << k << " множеств (сумма " << target_sum << "):" << endl;
        for (int i = 0; i < k; ++i) {
            cout << "{ ";
            for (int j = 0; j < (int)nums.size(); ++j) {
                if (groups[j] == i) cout << nums[j] << " ";
            }
            cout << "}" << endl;
        }
    } else {
        cout << "Невозможно разбить на подмножества с суммой " << target_sum << endl;
    }
}