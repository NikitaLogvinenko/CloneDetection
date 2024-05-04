#include "Bar_chart.hpp"
#include "Exceptions.hpp"
#include <exception>
#include <algorithm>

BarChart::BarChart (float min, float max, size_t count){
    min_border = min;
    max_border = max;
    bin_count = count;
    size_bin = (max-min)/bin_count;
    std::fill_n(std::back_inserter(bar_chart), bin_count, 0);
}

BarChart::BarChart (float min, float max, size_t count, std::istream &stream) : BarChart(min, max, count){
    float value;
    while(!stream.eof()){
        stream >> value;
        insertElem(value);
    }
}

BarChart::BarChart (float min, float max, size_t count, const std::vector<float> &vec) : BarChart(min, max, count){
    for (auto elem : vec){
        insertElem(elem);
    }
}

void BarChart::insertElem (float elem){
    float new_elem = std::clamp(elem, min_border, max_border-size_bin/2);
    size_t it_bin = new_elem/size_bin;
    ++bar_chart[it_bin];
}

void BarChart::changeBin(size_t number_bin, int count){
    bar_chart[number_bin] = count < 0 ? 0 : count;
}


float BarChart::getMin() const{
    return min_border;
}
float BarChart::getMax() const{
    return max_border;
}
size_t BarChart::getBinCount() const{
    return bin_count;
}
const std::vector<size_t>& BarChart::getBarChart() {
    return bar_chart;
}


std::vector <size_t>::iterator BarChart::begin(){
    return bar_chart.begin();
}
std::vector <size_t>::iterator BarChart::end(){
    return bar_chart.end();
}

bool BarChart::mainParamEqual(const BarChart& other) const {
    return (min_border == other.min_border && max_border == other.max_border && bin_count == other.bin_count);
}

BarChart BarChart::operator+ (const BarChart& other) const {
    if (!this->mainParamEqual(other))
        throw OperException();
    
    BarChart new_bar_chart (min_border, max_border, bin_count);
    for (size_t i = 0; i < bin_count; ++i){
        new_bar_chart.changeBin(i, (bar_chart.at(i) + other.bar_chart.at(i)));
    }
    return new_bar_chart;
}

BarChart BarChart::operator- (const BarChart& other) const{
    if (!this->mainParamEqual(other))
        throw OperException();
    
    BarChart new_bar_chart (min_border, max_border, bin_count);
    for (size_t i = 0; i < bin_count; ++i){
        new_bar_chart.changeBin(i, (bar_chart.at(i) - other.bar_chart.at(i)));
    }
    return new_bar_chart;
}

bool BarChart::operator== (const BarChart& other) const{
    if (!this->mainParamEqual(other))
        throw OperException();
    
    return bar_chart == other.bar_chart;
}
