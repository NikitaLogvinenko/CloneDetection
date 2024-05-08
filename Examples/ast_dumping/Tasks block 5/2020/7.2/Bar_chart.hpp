#ifndef Bar_chart_hpp
#define Bar_chart_hpp

#include <iostream>
#include <map>
#include <vector>


class BarChart {
public:
    
    BarChart (float min, float max, size_t bin_count, std::istream &stream);
    BarChart (float min, float max, size_t bin_count, const std::vector<float> &vec);
    BarChart (float min, float max, size_t bin_count);
    
    float getMin() const;
    float getMax() const;
    size_t getBinCount() const;
    const std::vector<size_t>& getBarChart();
    
    void insertElem (float elem);

    
    std::vector <size_t>::iterator begin();
    std::vector <size_t>::iterator end();
    
    BarChart operator+ (const BarChart& other) const;
    BarChart operator- (const BarChart& other) const;
    bool operator== (const BarChart& other) const;
    
private:
    float min_border;
    float max_border;
    size_t bin_count;
    float size_bin;
    std::vector <size_t> bar_chart;
    
    bool mainParamEqual (const BarChart& other) const;
    void changeBin (size_t number_bin, int count);
};



#endif /* Bar_chart_hpp */
