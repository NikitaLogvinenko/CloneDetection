#include "scientific_histogram.hpp"
#include <exception>
#include <algorithm>
#include <iterator>

using namespace std;

std::vector<int> get_vec(std::istream & stream_in)
{
	if (!stream_in) throw invalid_argument("Not valid input stream");
	vector<int> input_values;
	int value;
	while (stream_in >> value) input_values.push_back(value);
	return input_values;
}

bool equal_param(const ScientificHistogram & s_hist1, const ScientificHistogram & s_hist2)
{
	return s_hist1.get_min() == s_hist2.get_min() && s_hist1.get_max() == s_hist2.get_max() && s_hist1.get_bin_count() == s_hist2.get_bin_count();
}

ScientificHistogram::ScientificHistogram(int min, int max, int bin_count) : min(min), max(max), bin_count(bin_count)
{
	if (min > max || bin_count <= 0) throw invalid_argument("You got incorrect parameters in your constructor");
	for (int i = 0; i < bin_count; i++) bin_value_map.insert(pair<int, int>(i, 0));
}

ScientificHistogram::ScientificHistogram(int min, int max, int bin_count, const std::vector<int> & input_values) : ScientificHistogram(min,max,bin_count)
{
	auto map_it = bin_value_map.begin();
	int up_border = std::min(max - min, bin_count - 1);
	for (auto vec_it = input_values.begin(); vec_it != input_values.end(); vec_it++)
	{
		map_it = next(bin_value_map.begin(), (*vec_it - min <= up_border && *vec_it - min >= 0) ? (*vec_it - min) : (*vec_it - min > up_border ? up_border : 0));
		map_it->second++;
	}
}

ScientificHistogram::ScientificHistogram(int min, int max, int bin_count, std::istream & stream_in) : ScientificHistogram(min,max,bin_count,get_vec(stream_in)) {}

std::map<int, int>::const_iterator ScientificHistogram::cbegin() const
{
	return bin_value_map.cbegin();
}

std::map<int, int>::const_iterator ScientificHistogram::cend() const
{
	return bin_value_map.cend();
}

int ScientificHistogram::get_min() const
{
	return min;
}

int ScientificHistogram::get_max() const
{
	return max;
}

int ScientificHistogram::get_bin_count() const
{
	return bin_count;
}

ScientificHistogram ScientificHistogram::operator + (const ScientificHistogram & another_s_hist) const
{
	if(!equal_param(*(this),another_s_hist)) throw invalid_argument("Different parameters of scientific histograms");
	ScientificHistogram s_hist = *this;
	auto it2 = another_s_hist.cbegin();
	for (auto it1 = s_hist.bin_value_map.begin(); it1 != s_hist.bin_value_map.end(); it1++, it2++) { it1->second += it2->second; }
	return s_hist;
}

ScientificHistogram ScientificHistogram::operator - (const ScientificHistogram & another_s_hist) const
{
	if (!equal_param(*(this), another_s_hist)) throw invalid_argument("Different parameters of scientific histograms");
	ScientificHistogram s_hist = *this;
	auto it2 = another_s_hist.cbegin();
	for (auto it1 = s_hist.bin_value_map.begin(); it1 != s_hist.bin_value_map.end(); it1++, it2++)
	{
		it1->second = it1->second - it2->second >= 0 ? it1->second - it2->second : 0;
	}
	return s_hist;
}

bool ScientificHistogram::operator == (const ScientificHistogram & another_s_hist) const
{
	if (!equal_param(*(this), another_s_hist)) return false;
	for (auto it1 = this->cbegin(), it2 = another_s_hist.cbegin(); it1 != this->cend(); it1++, it2++)
	{
		if (it1->second != it2->second) return false;
	}
	return true;
}

void ScientificHistogram::add_el(int value)
{
	int up_border = std::min(max - min, bin_count - 1);
	auto map_it = bin_value_map.begin();
	map_it = next(bin_value_map.begin(), (value - min <= up_border && value - min >= 0) ? (value - min) : (value - min > up_border ? up_border : 0));
	map_it->second++;
}