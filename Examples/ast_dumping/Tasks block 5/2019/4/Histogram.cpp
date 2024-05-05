#include "Histogram.h"
#include  <iterator>

Histogram::Histogram(std::istream& istream)//Конструктор от istream
{
    std::string s;

    while (istream >> s)//Считывание слова из входного потока. Если его нет, map сам добавит его в  table с значением =0. Иначе, просто увеличит счётчик слов на 1.
    {
        table[s]++;
    }
}

Histogram::Histogram(const Histogram& h1)//Конструктор копирования
{
    for (auto it = h1.begin(); it != h1.end(); ++it)//Используя begin() и end() проходим по другой гистограмме и добавляем в нашу.
    {
        table[it->first] = it->second;
    }
}

Histogram::Histogram(std::vector<std::string> strings_vec)//Конструктор от vector<srting>
{
    for (std::string s : strings_vec)//Перебираем vector<string>. Берём слово s.
        //Если его нет, map сам добавит его в  table с значением = 0. Иначе, просто увеличит счётчик слов на 1.
    {
        table[s]++;
    }
}

int& Histogram::operator[](std::string s)//Оператор дотупа к элементу
{
    return table[s];
}


const Histogram Histogram::operator+(const Histogram& right) {//Оператор сложения
    Histogram* res;//Наибольшая гистограма

    //Определение наибольшей гистограмы
    if (getSize() > right.getSize())
    {
        res = new  Histogram(*this);
    } else
    {
        res = new Histogram(right);
    }

    for (auto it = res->table.begin(); it != res->table.end(); ++it)//Для каждого слово, что есть там проверяем, есть ли оно в другой гистограмме.
        //если есть, то добавляем его значения(значение в другой гистограмме) к нашей.
    {
        Histogram::citerator resf = right.find(it);
        if (resf != right.end())
        {

            (*res)[it->first] += resf->second;
        }
    }

    return *res;
}

const Histogram Histogram::operator-(const Histogram& right) {//Оператор вычитания
    Histogram* res;//Наибольшая гистограма

    //Определение наибольшей гистограмы
    if (getSize() > right.getSize())
    {
        res = new  Histogram(*this);
    } else
    {
        res = new Histogram(right);
    }

    for (auto it = res->table.begin(); it != res->table.end(); ++it)
    {
        auto resf = right.find(it);
        if (resf != right.end())
        {
            if (((*res)[it->first] - resf->second) < 0) {
                (*res)[it->first] = 0;//Количество слов не может быть отрицательным
            } else { (*res)[it->first] -= resf->second; }
        }
    }
    return *res;
}

Histogram& Histogram::operator-=(const Histogram& right) {//Оператор уменьшить на...
    for (auto it = begin(); it != end(); ++it)
    {
        auto resf = right.find(it);
        if (resf != right.end())
        {
            if ((table[it->first] - resf->second) < 0) {
                table[it->first] = 0;  //Количество слов не может быть отрицательным.
            } else { table[it->first] -= resf->second; }
        }
    }
    return *this;
}

Histogram& Histogram::operator+=(const Histogram& right) {//Оператор увеличить на
    for (auto it = begin(); it != end(); ++it)
    {
        auto resf = right.find(it);
        if (resf != right.end())
        {
            table[it->first] += resf->second;
        }
    }
    return *this;
}

bool operator==(const Histogram& left, const Histogram& right) {//Оператор сравнения на равенство

    if (left.table.size() != right.table.size())
    {
        return false;//Если размеры разные, то left и right точно не совпадают 
    }

    for (auto it = left.table.begin(); it != left.table.end(); ++it)
    {
        auto resf = right.table.find(it->first);

        if (resf == right.table.end())
        {
            return false;
        }
    }

    return true;
}