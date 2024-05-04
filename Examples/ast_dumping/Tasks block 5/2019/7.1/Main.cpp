#include <iostream>
#include "Histogram.h"

using namespace std;

int main()
{
	Histogram h1;
	/*h1.add(3);
	h1.add(3);
	h1.add(3);
	h1.add(3);
	h1.add(3);
	h1.add(6);
	h1.add(6);
	h1.add(7);*/
	for (int i = 0; i < 100; i++)
		h1.add(rand() / 1000);

	h1.draw("histogram.bmp", 500, 500, { 255, 250, 153 });

	system("pause");

	Histogram h2;
	h2.add(6);
	h2.add(6);
	h2.add(6);
	h2.add(6);
	h2.add(3);
	h2.add(3);
	h2.add(7);
	h2.add(7);
	h2.add(7);
	h2.add(5);
	h2.add(5);
	h2.add(8);

	Histogram h3;
	h3.add(3);
	h3.add(3);
	h3.add(3);
	h3.add(3);
	h3.add(3);
	h3.add(6);
	h3.add(6);
	h3.add(7);

	Histogram h4 = h1 + h2;

	//h4.draw();

	system("pause");
}