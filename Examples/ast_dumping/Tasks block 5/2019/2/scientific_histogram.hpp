#pragma once

#include <istream>;
#include <map>
#include <vector>

class ScientificHistogram {
public:
	ScientificHistogram(int min, int max, int bin_count);
	ScientificHistogram(int min, int max, int bin_count, std::istream & stream_in);
	ScientificHistogram(int min, int max, int bin_count, const std::vector<int> & input_values);

	ScientificHistogram operator + (const ScientificHistogram & another_s_hist) const; 
	ScientificHistogram operator - (const ScientificHistogram & another_s_hist) const;
	bool operator == (const ScientificHistogram & another_s_hist) const;

	std::map<int, int>::const_iterator cbegin() const;
	std::map<int, int>::const_iterator cend() const;

	int get_min() const;
	int get_max() const;
	int get_bin_count() const;

	void add_el(int value);

private:
	std::map<int, int> bin_value_map;
	int min;
	int max;
	int bin_count;
};