#include "Histogram.h"
#include <iostream>
#include <fstream>


int main() {

    std::ifstream f("1.txt");
    Histogram hs(f);

    /*
    for (auto it = table.begin(); it != table.end(); ++it){
        std::cout << it->first << " : " << it->second << std::endl;
    }
    */
    system("pause");
    return 0;
}