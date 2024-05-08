#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include "parse_string.h"
#include <vector>
#include "Histogram.hpp"
#include "hist_bitmap.h"

int main(int argc, char*argv[])
{
	std::cout << "Please enter the min\nmax value for histogram\nscale\n>";
	int min_bin, max_bin;
	size_t scale;
	std::cin >> min_bin;
	std::cin>>max_bin;
	std::cin >> scale;

	std::cout << "Please enter the filename\n>";
	std::string s;
	std::getline(std::cin >> std::ws, s);

	std::ifstream input_file(s);
	if (!input_file)
	{
		std::cerr << "File not found!" << std::endl;
		return 1;
	}
	Histogram h(min_bin, max_bin, input_file);
	Histogram h2(-34, 42, "1, 2, 2, 2, 1, 3,3,3,3,3,3,3,3");
	h -= h2;
	bool res = h.save_to_bitmap("out.bmp", scale);
	if (res == true)
		return 0;
	else
		return 1;
}

