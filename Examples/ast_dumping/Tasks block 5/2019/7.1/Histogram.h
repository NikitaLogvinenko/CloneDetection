#pragma once
#include <vector>
#include <string>
#include <map>
#include "BMPFigure.hpp"

class Histogram {
private:
	std::map <int, int> histogram;
public:
	typedef std::map<int, int>::const_iterator const_iterator;
	Histogram();
	Histogram(std::istream& in);
	Histogram(const std::vector<int>& in);
	Histogram(const std::map<int, int>& in);
	Histogram(const Histogram& h);

	const_iterator begin();
	const_iterator end();
	int size();
	int value(int key);
	void add(int key);
	void draw(std::string fname = "histogram.bmp", int width = 500, int height = 500, rgb fill = {155, 155, 155});
	void erase(int key);
	void reduce(int key);

	Histogram operator-(const Histogram& h) const;
	Histogram operator+(const Histogram& h) const;
	bool operator==(const Histogram& h) const;
};