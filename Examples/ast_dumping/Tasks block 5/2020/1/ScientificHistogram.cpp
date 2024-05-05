#include "ScientificHistogram.hpp"
#include <iostream>
#include <math.h>
#include <iomanip>
#include <exception>

ScientificHistogram::ScientificHistogram(double min, double max, size_t _bin):min_(min), max_(max), bin(_bin)
{
	if (bin == 0 || min >= max) { throw Incorrect_Arguments(); }
	barchar.resize(bin, 0);
	step = (max - min) / (double)bin;
};

void ScientificHistogram::Create(std::vector<double>& data)
{
	for (auto it : data)
	{
		int x =floor((it - min_) / step);

		if (it < min_ + step)
		{
			barchar[0]++;
			continue;
		}
		if (it > max_ - step)
		{
			barchar[bin - 1]++;
			continue;
		}
		barchar[x] ++;
	}
}
void ScientificHistogram::Create(std::istream& file)
{
	std::vector<double> data = {};
	Load(data, file);
	Create(data);
}

void ScientificHistogram::Print(std::ostream& out) const
{
	for (size_t i = 0; i < bin; i++)
	{
		out << barchar[i] << " ";
	}
	out << std::endl ;
}
bool ScientificHistogram::compare_state(const ScientificHistogram& bar)const
{
	return (bar.bin == bin && bar.max_ == max_ && bar.min_ == min_);
}

bool ScientificHistogram::operator==(const ScientificHistogram& bar)
{
	if (!compare_state(bar)) { throw Different_State(); }
	for (int i = 0; i < bin; i++)
	{
		if (barchar[i] != bar.barchar[i])
		{
			return false;
		}
	}
	return true;
}
bool ScientificHistogram::operator!=(const ScientificHistogram& bar)
{
	return !(*this==bar);
}
ScientificHistogram& ScientificHistogram::operator+=(const ScientificHistogram& bar)
{
	if (!compare_state(bar)) { throw Different_State(); }
	for (int i = 0; i < bin; i++)
	{
		barchar[i] = barchar[i] + bar.barchar[i];
	}
	return *this;
}
ScientificHistogram& ScientificHistogram::operator-=(const ScientificHistogram& bar)
{
	if (!compare_state(bar)) { throw Different_State();}
	for (int i = 0; i < bin; i++)
	{
		barchar[i] = barchar[i] - bar.barchar[i];
		if (barchar[i] < 0)
		{
			barchar[i] = 0;
		}
	}
	return *this;
}
ScientificHistogram ScientificHistogram::operator- (const ScientificHistogram& bar)const
{
	if (!compare_state(bar)) { throw Different_State(); }
	auto histogram(*this); 
	histogram -= bar;
	return histogram;
}
ScientificHistogram ScientificHistogram::operator+ (const ScientificHistogram& bar)const
{
	if (!compare_state(bar)) { throw Different_State();}
	auto histogram(*this);
	histogram += bar;
	return histogram;
}
double ScientificHistogram::operator[](size_t index) const
{
	if (index < 0 || index >= bin) { throw Out_of_Range(); }
	return barchar[index];
}

const std::vector<int>& ScientificHistogram::vector() const
{
	return barchar;
}
void ScientificHistogram::Load(std::vector<double>& data,std::istream& file)
{
	double elem = 0;
	data.clear();
	while (file >> elem)
	{
		data.push_back(elem);
	}
}

ScientificHistogram::citerator ScientificHistogram::begin()const
{
	return barchar.begin();
}
ScientificHistogram::citerator ScientificHistogram::end()const
{
	return barchar.end();
}

double ScientificHistogram::GetMax()const
{
	return max_;
}
double ScientificHistogram::GetMin()const
{
	return min_;
}
size_t ScientificHistogram::GetBin()const
{
	return bin;
}
double ScientificHistogram::GetStep()const
{
	return step;
}