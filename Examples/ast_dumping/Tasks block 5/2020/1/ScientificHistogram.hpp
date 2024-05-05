#pragma once
#include <vector>
#include <fstream>

class Histogram_Exception : public std::exception {
public:
	Histogram_Exception() = delete;
	Histogram_Exception(const char* what_) : std::exception(what_) {}
};

class Different_State : public Histogram_Exception
{
public:
	Different_State() : Histogram_Exception("Different State") {}
};

class Incorrect_Arguments : public Histogram_Exception
{
public:
	Incorrect_Arguments() : Histogram_Exception("Incorrect Arguments") {}
};

class Out_of_Range : public Histogram_Exception
{
public:
	Out_of_Range() : Histogram_Exception("Out of Range") {}
};

class ScientificHistogram
{
public:

	using citerator = std::vector<int>::const_iterator;

	ScientificHistogram(double min, double max, size_t _bin);

	void Create(std::vector<double>& prepare);
	void Create(std::istream& file);

	void Print(std::ostream& out)const;

	ScientificHistogram& operator+=(const ScientificHistogram& bar);
	ScientificHistogram& operator-=(const ScientificHistogram& bar);
	ScientificHistogram operator + (const ScientificHistogram& bar)const;
	ScientificHistogram operator - (const ScientificHistogram& bar)const;
	bool operator== (const ScientificHistogram& bar);
	bool operator!= (const ScientificHistogram& bar);
	double operator[](size_t index)const;

	citerator begin()const;
	citerator end()const;

	double GetMax()const;
	double GetMin()const;
	double GetStep()const;
	size_t GetBin()const;

	const std::vector<int>& vector()const;

private:
	bool compare_state(const ScientificHistogram& bar)const;
	void Load(std::vector<double>& data,std::istream& file);
	double max_ = 0.0;
	double min_ = 0.0;
	size_t bin{};
	double step{};
	std::vector<int> barchar;
};
