#include <iostream>
#include "Histogram.h"
#include <vector>

int main() {
	std::vector<int> a;
	for (int i = 0; i <= 10; i++) {
		a.push_back(i);
	}
	Histogram R(0, 10);
	Histogram t(0, 10);
	Histogram H(0, 10, a);
	try {
		t = R - H;
	}
	catch (std::exception& err) {
		std::cout << err.what() << std::endl;
	}
	
	std::cout << H.integrate()  << std::endl;
	system("pause");
	return 0;
}