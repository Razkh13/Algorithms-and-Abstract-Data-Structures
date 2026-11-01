#include "dbms.h"
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
    string filename, query;
    bool help = false;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--file"  && i + 1 < argc) filename = argv[++i];
        if (arg == "--query" && i + 1 < argc) query    = argv[++i];
        if (arg == "--help") help = true;
    }

    if (help || argc == 1) {
        cout << "СТЕК (S):\n";
        cout << "  SPUSH <имя> <значение>   — добавить\n";
        cout << "  SPOP <имя>               — удалить верхний\n";
        cout << "  SPRINT <имя>             — вывести\n";
        cout << "  SFIND <имя> <значение>   — поиск\n\n";
        cout << "  SVALIDATE <имя> <строка> — проверить XML\n";
        cout << "  SFIXXML <имя> <строка>   — восстановить XML (один символ испорчен)\n";

        cout << "МАССИВ (M):\n";
        cout << "  MPUSH <имя> <значение>              — добавить в конец\n";
        cout << "  MPUSHAT <имя> <индекс> <значение>   — вставить по индексу\n";
        cout << "  MPOP <имя>                          — удалить с конца\n";
        cout << "  MPOPAT <имя> <индекс>               — удалить по индексу\n";
        cout << "  MGET <имя> <индекс>                 — получить\n";
        cout << "  MSET <имя> <индекс> <значение>      — заменить\n";
        cout << "  MSIZE <имя>                         — длина\n";
        cout << "  MFIND <имя> <значение>              — поиск\n";
        cout << "  MPRINT <имя>                        — вывести\n";
        cout << "  MSUBARRAYS <имя>                    — вывести подмассивы\n\n";

        cout << "ОДНОСВЯЗНЫЙ СПИСОК (F):\n";
        cout << "  FPUSH <имя> <значение>                 — в голову\n";
        cout << "  FPUSHTAIL <имя> <значение>             — в хвост\n";
        cout << "  FPUSHBEFORE <имя> <цель> <значение>    — до элемента\n";
        cout << "  FPUSHAFTER <имя> <цель> <значение>     — после элемента\n";
        cout << "  FPOP <имя>                             — удалить голову\n";
        cout << "  FPOPTAIL <имя>                         — удалить хвост\n";
        cout << "  FPOPBEFORE <имя> <цель>                — удалить до\n";
        cout << "  FPOPAFTER <имя> <цель>                 — удалить после\n";
        cout << "  FPOPVALUE <имя> <значение>             — удалить по значению\n";
        cout << "  FFIND <имя> <значение>                 — поиск\n";
        cout << "  FPRINT <имя>                           — вывести\n";
        cout << "  FPRINTREVERSE <имя>                    — в обратном порядке\n";
        cout << "  FPRINTEVERYSECOND <имя>                — каждый второй\n\n";

        cout << "ДВУСВЯЗНЫЙ СПИСОК (L):\n";
        cout << "  LPUSH, LPUSHTAIL, LPUSHBEFORE, LPUSHAFTER,\n";
        cout << "  LPOP, LPOPTAIL, LPOPBEFORE, LPOPAFTER, LPOPVALUE,\n";
        cout << "  LFIND, LPRINT, LPRINTREVERSE, LPRINTEVERYSECOND\n\n";

        cout << "ОЧЕРЕДЬ (Q):\n";
        cout << "  QPUSH <имя> <значение>   — добавить\n";
        cout << "  QPOP <имя>               — удалить из головы\n";
        cout << "  QPRINT <имя>             — вывести\n";
        cout << "  QFIND <имя> <значение>   — поиск\n\n";

        cout << "AVL-ДЕРЕВО (T):\n";
        cout << "  TINSERT <имя> <значение> — вставить\n";
        cout << "  TDEL <имя> <значение>    — удалить\n";
        cout << "  TGET <имя> <значение>    — поиск\n";
        cout << "  ISMEMBER <имя> <значение> — поиск\n";
        cout << "  TPRINT <имя>             — вывести\n";
        cout << "  THEIGHT <имя>            — высота\n";
        cout << "  THUFFMAN <строка>        — кодировать по Хаффману\n";
        cout << "  THUFFMANDEC <код> <таблица> — декодировать\n\n";

        cout << "МНОЖЕСТВО (SET):\n";
        cout << "  SETADD <имя> <значение>  — добавить\n";
        cout << "  SETDEL <имя> <значение>  — удалить\n";
        cout << "  SET_AT <имя> <значение>  — проверить наличие\n";
        cout << "  SETPARTITION <имя> <k>   — разбить на k подмножеств\n\n";

        cout << "ХЕШ-ТАБЛИЦА (HT):\n";
        cout << "  HTADD <имя> <метод> <значение> — добавить (метод 1 или 2)\n";
        cout << "  HTDEL <имя> <значение>         — удалить\n";
        cout << "  HTGET <имя> <значение>         — проверить наличие\n";
        cout << "  HTLONGEST <строка>             — макс. подстрока без повторов\n\n";

        cout << "LRU-КЭШ:\n";
        cout << "  LRUSET <имя> <емкость> <ключ> <значение> — добавить\n";
        cout << "  LRUGET <имя> <ключ>                      — получить\n\n";

        cout << "ОБЩЕЕ:\n";
        cout << "  PRINT <имя>              — вывести любую структуру\n";
        cout << "  --file <файл>            — файл данных\n";
        cout << "  --query '<команда>'      — выполнить команды (разделитель ;)\n";
        cout << "  --help                   — эта справка\n";
        return 0;
    }

    if (filename.empty() || query.empty()) {
        cout << "Использование: ./dbms --file <файл> --query '<КОМАНДА>'\n";
        cout << "Справка: ./dbms --help\n";
        return 1;
    }

    DBMS db(filename);
    db.LoadAll();

    size_t start = 0;
    size_t end = query.find(';');
    while (end != string::npos) {
        string one_query = query.substr(start, end - start);

        size_t first = one_query.find_first_not_of(" \t\r\n");
        size_t last = one_query.find_last_not_of(" \t\r\n");
        if (first != string::npos && last != string::npos) {
            one_query = one_query.substr(first, last - first + 1);
            db.ExecuteQuery(one_query);
        }
        
        start = end + 1;
        end = query.find(';', start);
    }

    string last_query = query.substr(start);
    size_t first = last_query.find_first_not_of(" \t\r\n");
    size_t last = last_query.find_last_not_of(" \t\r\n");
    if (first != string::npos && last != string::npos) {
        last_query = last_query.substr(first, last - first + 1);
        db.ExecuteQuery(last_query);
    }

    db.SaveAll();

    return 0;
}