#include "Histogram.h"
#include  <iterator>

Histogram::Histogram(std::istream& istream)//����������� �� istream
{
    std::string s;

    while (istream >> s)//���������� ����� �� �������� ������. ���� ��� ���, map ��� ������� ��� �  table � ��������� =0. �����, ������ �������� ������� ���� �� 1.
    {
        table[s]++;
    }
}

Histogram::Histogram(const Histogram& h1)//����������� �����������
{
    for (auto it = h1.begin(); it != h1.end(); ++it)//��������� begin() � end() �������� �� ������ ����������� � ��������� � ����.
    {
        table[it->first] = it->second;
    }
}

Histogram::Histogram(std::vector<std::string> strings_vec)//����������� �� vector<srting>
{
    for (std::string s : strings_vec)//���������� vector<string>. ���� ����� s.
        //���� ��� ���, map ��� ������� ��� �  table � ��������� = 0. �����, ������ �������� ������� ���� �� 1.
    {
        table[s]++;
    }
}

int& Histogram::operator[](std::string s)//�������� ������ � ��������
{
    return table[s];
}


const Histogram Histogram::operator+(const Histogram& right) {//�������� ��������
    Histogram* res;//���������� ����������

    //����������� ���������� ����������
    if (getSize() > right.getSize())
    {
        res = new  Histogram(*this);
    } else
    {
        res = new Histogram(right);
    }

    for (auto it = res->table.begin(); it != res->table.end(); ++it)//��� ������� �����, ��� ���� ��� ���������, ���� �� ��� � ������ �����������.
        //���� ����, �� ��������� ��� ��������(�������� � ������ �����������) � �����.
    {
        Histogram::citerator resf = right.find(it);
        if (resf != right.end())
        {

            (*res)[it->first] += resf->second;
        }
    }

    return *res;
}

const Histogram Histogram::operator-(const Histogram& right) {//�������� ���������
    Histogram* res;//���������� ����������

    //����������� ���������� ����������
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
                (*res)[it->first] = 0;//���������� ���� �� ����� ���� �������������
            } else { (*res)[it->first] -= resf->second; }
        }
    }
    return *res;
}

Histogram& Histogram::operator-=(const Histogram& right) {//�������� ��������� ��...
    for (auto it = begin(); it != end(); ++it)
    {
        auto resf = right.find(it);
        if (resf != right.end())
        {
            if ((table[it->first] - resf->second) < 0) {
                table[it->first] = 0;  //���������� ���� �� ����� ���� �������������.
            } else { table[it->first] -= resf->second; }
        }
    }
    return *this;
}

Histogram& Histogram::operator+=(const Histogram& right) {//�������� ��������� ��
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

bool operator==(const Histogram& left, const Histogram& right) {//�������� ��������� �� ���������

    if (left.table.size() != right.table.size())
    {
        return false;//���� ������� ������, �� left � right ����� �� ��������� 
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