#include <iterator>
#include <iostream>
#include "Histogram.h"
#include "BMPFigure.hpp"

using namespace std;

Histogram::Histogram()
{
	//
}

Histogram::Histogram(std::istream& in)
{
	while (in)
	{
		int key;
		in >> key;
		if (histogram.find(key) != histogram.end())
			histogram[key]++;
		else
			histogram[key] = 1;
	}
}

Histogram::Histogram(const std::vector<int>& in)
{
	for (auto i : in) {}
	for (auto it = in.cbegin(); it != in.cend(); it++)
		if (histogram.find(*it) != histogram.end())
			histogram[*it]++;
		else
			histogram[*it] = 1;
}

Histogram::Histogram(const Histogram& h)
{
	//std::map <int, int> h_h = h.histogram;
	//for (map<int, int>::iterator it = h_h.begin(); it != h_h.end(); it++)
	//	histogram[it->first] = it->second;
	histogram = h.histogram;
}

int Histogram::size()
{
	return histogram.size();
}

int Histogram::value(int key)
{
	if (histogram.find(key) == histogram.end())
		return 0;
	return histogram[key];
}

void Histogram::add(int key)
{
	if (histogram.find(key) != histogram.end())
		histogram[key]++;
	else
		histogram[key] = 1;
}

Histogram::Histogram(const map<int, int>& in)
{
	//for (map<int, int>::iterator it = in.begin();
	//	it != in.end(); it++)
	//	for (int i = 0; i < it->second; i++)
	//		add(it->first);
	histogram = in;
}

void Histogram::erase(int key)
{
	if (histogram.find(key) != histogram.end())
		histogram[key] = 0;
}

void Histogram::reduce(int key)
{
	if (histogram.find(key) != histogram.end())
		histogram[key]--;
}

Histogram::const_iterator Histogram::begin()
{
	return histogram.begin();
}

Histogram::const_iterator Histogram::end()
{
	return histogram.end();
}

void Histogram::draw(std::string fname, int width, int height, rgb fill)
{
	std::map<int, int>::iterator it = histogram.begin();
	int min_key = it->first;
	int max_key = it->first;
	int min_value = it->second;
	int max_value = it->second;
	for (it; it != histogram.end(); it++)
	{
		if (it->first > max_key)
			max_key = it->first;
		if (it->first < min_key)
			min_key = it->first;
		if (it->second > max_value)
			max_value = it->second;
		if (it->second < min_value)
			min_value = it->second;
	}
	BMPFigure bmp(width, height);
	bmp.background_color();
	bmp.draw_rect({ 0, 0 }, { width - 1, height - 1 });
	if (max_value == 0)
	{
		bmp.draw_rect({ 10, height - 10 }, { width - 10, height - 10 });
		bmp.save(fname);
		return;
	}
	double delta = (width - 20) / (max_key - min_key + 1);
	int scaleY = (height - 10) / max_value;

	for (it = histogram.begin(); it != histogram.end(); it++)
	{
		bmp.draw_rect(	{ static_cast<int>((it->first - min_key)*delta + 10), height - it->second*scaleY },
			{ static_cast<int>((it->first - min_key + 1)*delta + 10), height - 10 }, {0, 0, 0}, fill);
	}

	bmp.save(fname);
}

Histogram Histogram::operator-(const Histogram& h) const
{
	Histogram diff(*this);
	std::map <int, int> h_h = h.histogram;

	for (std::map<int, int>::iterator it = h_h.begin(); it != h_h.end(); it++)
		if (diff.histogram.find(it->first) != diff.histogram.end())
			diff.histogram[it->first] -= h_h[it->first];
		else
		{
			diff.histogram[it->first] = -h_h[it->first];
		}

	return diff;
}

Histogram Histogram::operator+(const Histogram& h) const
{
	Histogram summ(*this);
	std::map <int, int> h_h = h.histogram;

	for (std::map<int, int>::iterator it = h_h.begin(); it != h_h.end(); it++)
		if (summ.histogram.find(it->first) != summ.histogram.end())
			summ.histogram[it->first] += h_h[it->first];
		else
		{
			summ.histogram[it->first] = h_h[it->first];
		}

	return summ;
}

bool Histogram::operator==(const Histogram& h) const
{
	if (histogram.size() != h.histogram.size())
		return false;

	Histogram h0(*this); // ?
	std::map <int, int> h_h = h.histogram;
	for (std::map<int, int>::iterator it = h_h.begin(); it != h_h.end(); it++)
		if (h0.histogram[it->first] != it->second)
			return false;

	return true;
}