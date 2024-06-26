// Histogram.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "libbitmap.h"
#include "Histogram.hpp"
#include <iterator>
#include "parse_string.h"
#include "math.h"
#include "hist_bitmap.h"
#include "shapes.h"


int max(int a, int b);
int abs(int a);

Histogram::Histogram(int min_bin, int max_bin, std::istream& infile)
{
	std::string s;
	std::getline(infile, s);
	std::vector<int> data = parse_string(s);
	bool res = init(min_bin, max_bin, data);
}

Histogram::Histogram(int _min_bin, int _max_bin, const std::vector<int> v)
{
	bool res= init(_min_bin, _max_bin, v);
}

Histogram::Histogram(int min_bin, int max_bin, const std::string s)
{
	std::vector<int> data = parse_string(s);
	bool res = init(min_bin, max_bin, data);
}

Histogram & Histogram::operator+=(const Histogram & right)
{
	if (max_bin == right.max_bin && min_bin == right.min_bin)
	{
		size_t counter = 0;
		for (auto& left_val : right.bins_height)
		{
			bins_height[counter++] += left_val;
		}
	}
	else
		throw InvalidBoundsException();
	return *this;
}

Histogram & Histogram::operator-=(const Histogram & right)
{
	if (max_bin == right.max_bin && min_bin == right.min_bin)
	{
		size_t counter = 0;
		for (auto& left_val : right.bins_height)
		{
			bins_height[counter++] -= left_val;
		}
	}
	else
		throw InvalidBoundsException();
	return *this;
}

Histogram & Histogram::operator=(const Histogram & right)
{
	if (&right == this)
		return *this;

	max_bin = right.max_bin;
	min_bin = right.min_bin;
	bins_height = right.bins_height;
	return *this;
}

Histogram Histogram::operator+(const Histogram & right) const
{
	Histogram z(*this);
	z += right;
	return z;
}

Histogram Histogram::operator-(const Histogram & right) const
{
	Histogram z(*this);
	z -= right;
	return z;
}


bool Histogram::operator==(const Histogram & right) const
{
	return (min_bin==right.min_bin && max_bin==right.max_bin && bins_height==right.bins_height);
}

Histogram::iterator Histogram::begin() const
{
	return bins_height.cbegin();
}

Histogram::iterator  Histogram::end() const
{
	return bins_height.cend();
}

size_t Histogram::operator[](int index) const
{
	if (index >= min_bin)
	{
		return bins_height[index - min_bin];
	}
	else
		throw InvalidBoundsException();
}

const int Histogram::get_min() const {return min_bin;}

const int Histogram::get_max() const {return max_bin;}

const int Histogram::get_max_height() const
{
	int max_height = *begin();
	for (auto it = begin(); it != end(); ++it)
	{
		if (*it > max_height)
		{
			max_height=*it;
		}
	}
	return max_height;
}

const int Histogram::get_min_height() const
{
	int min_height = *begin();
	for (auto it = begin(); it != end(); ++it)
	{
		if (*it < min_height)
		{
			min_height = *it;
		}
	}
	return min_height;
}

const int Histogram::get_bin_count() const {return max_bin-min_bin+1;}

bool Histogram::save_to_bitmap(const std::string filename, int scale)
{
	
	if (scale <= 0)
	{
		scale = 1;
	}
	bool res;
	int h_min = mmin(0, get_min_height());
	int h_max = max(0, get_max_height());
	int h_height = h_max - h_min;

	Point local_zero = { abs(min_bin), -h_min };// h_height - abs(h_min)


	hist_Bitmap bitmap(scale*get_bin_count(), scale*h_height);
	
	int j = 0;
	Rectangle rect;
	for (auto it = begin(); it != end(); ++it, ++j)
	{
		rect = { { scale*j, (h_height - (local_zero.y + *it)) * scale }, { scale*(j + 1), (h_height - local_zero.y) * scale } };
		res = bitmap.draw_rect(rect);
		if (res == false)
			return false;
	}
	if (min_bin <= 0 && 0 <= max_bin)
	{
		res = bitmap.draw_v(local_zero.x * scale + scale / 2, 0, bitmap.m_height);
		if (res == false)
			return false;
	}
	if(local_zero.y!=0)
		res = bitmap.draw_h(0, bitmap.m_width, (h_height - local_zero.y) * scale-1); //если не отнимать 1, запись за пределы массива - ошибка
	else
		res = bitmap.draw_h(0, bitmap.m_width, 0); //не подходит под отрисовку по 0 координате, при -1 - отрицательный индекс
	if (res == false)
	{
		return false;
	}
	return bitmap.save(filename);
}

bool Histogram::init(int _min_bin, int _max_bin, const std::vector<int> data)
{
	min_bin = _min_bin;
	max_bin = _max_bin;
	bins_height.resize(max_bin - min_bin + 1); //выделение памяти
	if (min_bin == max_bin)
	{
		bins_height[0] += data.size();
	}
	else
	{
		for (const auto& it : data)
		{
			if (it >= _max_bin)
			{
				bins_height[bins_height.size() - 1]++;
			}
			if (it <= _min_bin)
			{
				bins_height[0]++;
			}
			else if (it > _min_bin && it < _max_bin)
			{
				bins_height[(it - min_bin)]++; //fabs wtf но abs не работает :(
			}
		}
	}
	
	return true;
}

Histogram::Histogram(const Histogram & right)
{
	max_bin = right.max_bin;
	min_bin = right.min_bin;
	bins_height = right.bins_height;
}
