#ifndef __HISTOGRAM_H__
#define __HISTOGRAM_H__

#include <iostream>
#include <vector>
#include <strstream>
//#include "hist_bitmap.h"
class hist_Bitmap;

class Histogram 
{

public:
	class InvalidBoundsException :public std::exception {
	public:
		InvalidBoundsException() :std::exception("Invalid Bounds") {}
	};
	Histogram() = delete;
	Histogram(const Histogram &right);//+
	Histogram(int min_bin, int max_bin, std::istream& infile);//+
	Histogram(int min_bin, int max_bin, const std::vector <int> v);//+ 
	Histogram(int min_bin, int max_bin, const std::string s); //+

	
	Histogram& operator+=(const Histogram &right);//+
	Histogram& operator-=(const Histogram &right);//+
	Histogram& operator=(const Histogram &right);//+

	Histogram operator+(const Histogram &right) const;//+
	Histogram operator-(const Histogram &right) const;//+
	bool operator==(const Histogram &right) const;//+

	using  iterator=std::vector<int>::const_iterator;
	iterator begin() const; //+
	iterator  end() const; //+

	size_t operator[](int) const; //+

public: 
	const int get_min() const; //��������� �������� ��� ����
	const int get_max()const;//��������� �������� ���� ����
	const int get_max_height() const; //���� ������ �����������
	const int get_min_height() const; //��� ������ (������ ��������� ������������� ������ ���� ��������� -
	const int get_bin_count() const; //+
	static int max(int a, int b) { return a>b ? a : b; }
	static int mmin(int a, int b) { return a<b ? a : b; }
	static int abs(int a) { return a >= 0 ? a : -a; }

	
	bool save_to_bitmap(const std::string filename, int scale=50);
	
	/*min_bin - ����������� ��������. �������� min_bin=1 - ����� ��� �������� <=1 ��
	�������� ������ �������� � ������� ���, max_bin=7 - ����� ��� �������� >=7 �������� � [6] ���*/
	 //������� ��� "����� ���� - ���������� ���������"
	

private:
	int min_bin=0;
	int max_bin=0;
	std::vector<int> bins_height;
	bool init(int _min_bin, int _max_bin, std::vector <int> data);
	
};
#endif //__HISTOGRAM_H__

