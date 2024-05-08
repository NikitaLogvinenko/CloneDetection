#include "histogram.hpp"
#include <istream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <map>
#include <sstream>


void Histogram::add_el(const std::string &element) {
    if (vocab.find(element) != vocab.end()) {
        vocab[element]++;
    } else {
        vocab[element] = 1;
    }
}


void Histogram::init(std::vector<std::string> &book) {
    for (auto &it: book) {
        add_el(it);
    }
}


Histogram::Histogram(std::vector<std::string> &book) {
    init(book);
}


Histogram::Histogram(std::istream &stream) {
    std::vector<std::string> book;
    std::copy(std::istream_iterator<std::string>(stream), {}, back_inserter(book));
    init(book);
}


Histogram::Histogram(const Histogram &hist) {
    vocab = hist.vocab;
}


Histogram::~Histogram() {
    vocab.clear();
}


const Histogram operator+(const Histogram &left, const Histogram &right) {
    Histogram result(left);
    for (auto &it: right.vocab) {
        if (result.vocab.find(it.first) != result.vocab.end()) {
            result.vocab[it.first] += it.second;
        } else {
            result.vocab[it.first] = it.second;
        }
    }
    return result;
}


const Histogram operator-(const Histogram &left, const Histogram &right) {
    Histogram result(left);
    for (auto &it: right.vocab) {
        if (result.vocab.find(it.first) != result.vocab.end()) {
            result.vocab[it.first] -= it.second;
        } else {
            result.vocab[it.first] = -it.second;
        }
    }
    return result;
}


bool operator==(const Histogram &left, const Histogram &right) {
    return left.vocab == right.vocab;
}


Histogram &operator+=(Histogram &left, const Histogram &right) {
    left = left + right;
    return left;
}


Histogram &operator-=(Histogram &left, const Histogram &right) {
    left = left - right;
    return left;
}


const std::map<const std::string, int> get_hist(const Histogram &hist) {
    std::map<const std::string, int> result;
    std::copy(hist.vocab.begin(), hist.vocab.end(), std::inserter(result, result.end()));
    return result;
}