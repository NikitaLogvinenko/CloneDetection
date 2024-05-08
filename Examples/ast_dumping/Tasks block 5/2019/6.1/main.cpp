#include "histogram.hpp"
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

int main() {
	std::vector<std::string> h1 = { "1","2","2","2","2","59","3"};
	std::vector<std::string> h2 = { "2","3","4","5"};
	std::stringstream ss("1 2 55  642 5325 512 1 2 55" );

	Histogram Shist(ss);


	Histogram hist1(h1);
	Histogram hist2(h2);
	Histogram summa = hist2 + hist1;
	hist2 += hist1;

	bool check = hist2 == summa;

	Histogram minus = summa - hist2;
}