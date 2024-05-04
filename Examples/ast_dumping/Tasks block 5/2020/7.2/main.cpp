#include <iostream>
#include "Bar_chart.hpp"

int main(int argc, const char * argv[]) {
    
    std::vector <float> vec = {1,2,3,4,5,6,2,24,29};
    std::vector <float> vec2 = {1, 3, 6, 8};

    float min = 0;
    float max = 10;
    size_t bin_count = 4;

    BarChart first{min, max, bin_count, vec};
    BarChart second{min, max, bin_count, vec2};
    BarChart third{min, max, bin_count, vec};
    BarChart forth{min, max, bin_count};

    std::vector<size_t> bar = first.getBarChart();
    std::vector<size_t> bar2 = second.getBarChart();
    
    int i = 0;
    for (auto elem: bar){
        std::cout << "bin - " << i << " count - " << elem << std::endl;
        ++i;
    }
    i=0;
    for (auto elem: bar2){
        std::cout << "bin - " << i << " count - " << elem << std::endl;
        ++i;
    }
    i=0;

    second = first - second;
    
    bar = second.getBarChart();
    for (auto elem: bar){
        std::cout << "bin - " << i << " count - " << elem << std::endl;
        ++i;
    }
    i=0;
    
    for (int i = 0; i<first.getBinCount(); ++i){

        auto iter = first.begin();
        for (int j = 0; j < i; ++j)
            ++iter;

        size_t pair = *iter;

        std::cout << "iter " << i << " - " << pair << std::endl;
    }
    
    try {
        forth = third + first;
    } catch (std::exception& ex) {
        std::cout << ex.what();
    }

    return 0;
}
