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
    std::map<std::string, int> table;//C������ ��� ����� - ���������� ����������

public:
    using citerator = std::map<std::string, int> ::const_iterator;

    Histogram(std::istream&);//����������� �� istream

    Histogram(const Histogram& h1);//����������� �����������

    Histogram(std::vector<std::string>);//����������� �� vector<srting>

    citerator begin() const { return table.cbegin(); } //������ �������� �� ����������
    citerator end() const { return table.cend(); } //����� �������� �� ����������

    const Histogram operator+(const Histogram& right);//�������� � ������ ������������ 
    const Histogram operator-(const Histogram& right);//��������� ������ ����������� 
    friend bool operator==(const Histogram& left, const Histogram& right);//�������� ��������� ���� ���������� 

    Histogram& operator-=(const Histogram& right);//��������� ������ ����������� 
    Histogram& operator+=(const Histogram& right);//�������� ������ ����������� 

    int& operator[](std::string s);//������ � �������� ����������

    int const getSize() const { return table.size(); }
    citerator find(citerator i) const { return table.find(i->first); }

    //Histogram operator[](std::string s);
};
#endif

