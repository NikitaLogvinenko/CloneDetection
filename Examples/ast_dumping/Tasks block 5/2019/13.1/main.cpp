#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <ctime>
#include "obeder.hpp"


int main() {
	std::istream &is = std::cin;
	Obeder meal;
	meal.create_obed(is);
	output_dept(meal.vect_depts(1, 2), std::cout);
	getchar();
	return  0;
}