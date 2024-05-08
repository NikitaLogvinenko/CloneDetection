#pragma once
#include <vector>

class Histogram {
	typedef int data_type;
public:
	Histogram();
	Histogram(data_type _min, data_type _max);
	Histogram(data_type _min, data_type _max, std::vector<data_type> data);
	Histogram(data_type _min, data_type _max, std::istream &in);
	~Histogram();

	int max() const { return max_val; }
	int min() const { return min_val; }
	int bin() const { return bin_count; }
	typedef std::vector<data_type>::const_iterator citer;
	citer begin() const;
	citer end() const;

	size_t integrate();


	int operator [] (int i) const;
	Histogram& operator+= (const Histogram& b);
	Histogram& operator-= (const Histogram& b);
	Histogram& operator = (const Histogram& source);

	Histogram operator+(const Histogram& b) const;
	Histogram operator-(const Histogram& b) const;
	bool operator==(const Histogram& b) const;

private:
	int max_val;
	int min_val;
	int bin_count;
	std::vector<data_type> datalist;

private:
	bool is_bad(const Histogram &h1);
	void push(int value);
	void init(int max, int min);
};

class no_bin_ex: public std::exception {
public:
	no_bin_ex(int i) throw() :std::exception("no bin %d", i) {}
};

class diff_hist_ex : public std::exception {
public:
	diff_hist_ex() throw() :std::exception("different histograms") {}
};

class inc_val_ex : public std::exception {
public:
	inc_val_ex() throw() :std::exception("incorrect min and max value") {}
};


