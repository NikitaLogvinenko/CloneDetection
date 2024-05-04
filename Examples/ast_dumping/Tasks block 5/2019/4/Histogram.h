#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <istream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>

class Histogram
{
private:
    std::map<std::string, int> table;//Cловарь пар слово - количество повторений

public:
    using citerator = std::map<std::string, int> ::const_iterator;

    Histogram(std::istream&);//Конструктор от istream

    Histogram(const Histogram& h1);//Конструктор копирования

    Histogram(std::vector<std::string>);//Конструктор от vector<srting>

    citerator begin() const { return table.cbegin(); } //Начало итерации по гистограме
    citerator end() const { return table.cend(); } //Конец итерации по гистограме

    const Histogram operator+(const Histogram& right);//Сложение с другой гистограммой 
    const Histogram operator-(const Histogram& right);//Вычитание другой гистограммы 
    friend bool operator==(const Histogram& left, const Histogram& right);//Проверка равенства двух гистограмм 

    Histogram& operator-=(const Histogram& right);//Вычитание другой гистограммы 
    Histogram& operator+=(const Histogram& right);//Сложение другой гистограммы 

    int& operator[](std::string s);//доступ к элементу гистограмы

    int const getSize() const { return table.size(); }
    citerator find(citerator i) const { return table.find(i->first); }

    //Histogram operator[](std::string s);
};
#endif

