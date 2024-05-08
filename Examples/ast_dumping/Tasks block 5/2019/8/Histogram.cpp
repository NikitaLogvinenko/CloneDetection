#include "Histogram.h"
#include <exception>
#include <iostream>

typedef int data_type;

void Histogram::push(int value) {
	if (value > max_val) {
		datalist[bin_count - 1]++;
	}
	else {
		if (value < min_val) {
			datalist[0]++;
		}
		else {
			datalist[value - min_val]++;
		}
	}
}

void Histogram::init(int max, int min) {
	if (max < min) {
		throw inc_val_ex();
	}
	min_val = min;
	max_val = max;
	bin_count = max_val - min_val + 1;
	datalist.resize(bin_count);
}

Histogram::Histogram()
{
	init(0, 0);
}

Histogram::Histogram(data_type _min, data_type _max) {
	init(_max, _min);
}

Histogram::Histogram(data_type _min, data_type _max, std::vector<data_type> data) {
	init(_max, _min);
	for (std::vector<data_type>::iterator it = data.begin(); it != data.end(); ++it) {
		push(*it);
	}
}

Histogram::Histogram(data_type _min, data_type _max, std::istream & in) {
	init(_max, _min);
	int value;
	while (in >> value) {
		push((int)value);
	}
}

Histogram::~Histogram()
{
}

typedef std::vector<data_type>::const_iterator citer;

citer Histogram::begin() const {
	return datalist.begin();
}

citer Histogram::end() const {
	return datalist.end();
}

size_t Histogram::integrate() {
	size_t summ = 0;
	for (citer it = begin(); it != end(); it++) {
		summ += *it;
	}
	return summ;
}

int Histogram::operator[](int i) const {
	if (i < min_val) throw no_bin_ex(i);
	if (i > max_val) throw no_bin_ex(i);
	return datalist[i - min_val];
}

Histogram & Histogram::operator+=(const Histogram & b) {
	if (is_bad(b)) throw diff_hist_ex();
	for (int i = 0; i < b.bin_count; i++) {
		datalist[i] += b.datalist[i];
	}
	return *this;
}

Histogram & Histogram::operator-=(const Histogram & b)
{
	if (is_bad(b)) throw diff_hist_ex();
	for (int i = 0; i < b.bin_count; i++) {
		datalist[i] -= b.datalist[i];
	}
	return *this;
}

Histogram & Histogram::operator=(const Histogram & source)
{
	min_val = source.min();
	max_val = source.max();
	bin_count = source.bin();
	datalist = source.datalist;
	return *this; 
}

Histogram Histogram::operator+(const Histogram & b) const
{
	Histogram result(*this);
	return result += b;
}

Histogram Histogram::operator-(const Histogram & b) const {
	Histogram result(*this);
	return result -= b;
}

bool Histogram::operator==(const Histogram & b) const {
	if (datalist != b.datalist) return 0;
	return 1;
}

bool Histogram::is_bad(const Histogram & h1)
{
	if (min_val != h1.min()) return true; 
	if (max_val != h1.max()) return true;
	return false;
}
