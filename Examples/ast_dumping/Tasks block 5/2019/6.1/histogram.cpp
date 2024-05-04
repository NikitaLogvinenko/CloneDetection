#include "histogram.hpp"
#include <istream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <map>
#include <sstream>
void Histogram::add_el(const std::string& element) {
	if (vocab.find(element)!= vocab.end()) {
		vocab[element]++;
	}
	else {
		vocab[element] = 1;
	}
}
void Histogram::init(std::vector<std::string>& book) {
	for (auto it = book.begin(); it != book.end(); ++it) {
		add_el(*it);
	}
}
Histogram::Histogram(std::vector<std::string>& book) {
	init(book);
}
Histogram::Histogram(std::istream& stream) {
	std::vector<std::string> book;
	std::copy(std::istream_iterator<std::string>(stream), {}, back_inserter(book));
	init(book);
}
Histogram::Histogram(const Histogram& hist) {
	vocab = hist.vocab;
}
Histogram::	~Histogram() {
	vocab.clear();
}
Histogram operator+(const Histogram& left, const Histogram& right) {
	Histogram result(left);
	result += right;
	return result;
}
 Histogram operator-(const Histogram& left, const Histogram& right) {
	Histogram result(left);
	result -= right;
	return result;
}

bool operator==(const Histogram& left, const Histogram& right) {
	return left.vocab == right.vocab;
}
void Histogram::operator-=(const Histogram& right) {
	for (auto it = right.vocab.begin(); it != right.vocab.end(); ++it) {
		if (vocab.find(it->first) != vocab.end()) {
			vocab[it->first] -= it->second;
		}
		else {
			vocab[it->first] = -it->second;
		}
	}
}
void Histogram::operator+=(const Histogram& right) {
	for (auto it = right.vocab.begin(); it != right.vocab.end(); ++it) {
		if (vocab.find(it->first) != vocab.end()) {
			vocab[it->first] += it->second;
		}
		else {
			vocab[it->first] = it->second;
		}
	}
}
const std::map<const std::string, int>& Histogram::get_hist() {
	return vocab;
}