#ifndef HISTOGRAM_HISTOGRAM_HPP
#define HISTOGRAM_HISTOGRAM_HPP

#include <vector>
#include <map>


class Histogram {
public:
    typedef std::map<std::string, int>::const_iterator hist_iterator;

    Histogram() {};

    Histogram(std::istream &stream);

    Histogram(std::vector<std::string> &book);

    Histogram(const Histogram &hist);

    ~Histogram();

    friend const std::map<const std::string, int> get_hist(const Histogram &hist);

    friend const Histogram operator+(const Histogram &left, const Histogram &right);

    friend const Histogram operator-(const Histogram &left, const Histogram &right);

    friend bool operator==(const Histogram &left, const Histogram &right);

    friend Histogram &operator+=(Histogram &left, const Histogram &right);

    friend Histogram &operator-=(Histogram &left, const Histogram &right);

    void add_el(const std::string &element);

    hist_iterator begin() { return vocab.cbegin(); };

    hist_iterator end() { return vocab.cend(); };
private:
    std::map<std::string, int> vocab;

    void init(std::vector<std::string> &book);
};


#endif //HISTOGRAM_HISTOGRAM_HPP
