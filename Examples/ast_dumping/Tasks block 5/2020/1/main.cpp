#include "ScientificHistogram.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>

using namespace std;

void error()
{
	cout << "Error" << endl;
}

int main(int argc, char* argv[])
{
	ifstream file;
	if (argc >= 3)return 1;
	else if (argc == 2)
	{
		file.open(argv[1]);
	}
	else
	{
		string adress = "4.txt";
		cout << "Write anagram adress (1,2,3,4.txt): ";
		cin >> adress;
		file.open(adress);
	}
	if (!file)
	{
		error();
		system("pause");
		return 2;
	}
	double max = 10;
	double min = 1;
	size_t bin = 9;


	cout << "Write min: ";
	cin >> min;
	cout << "Write max: ";
	cin >> max;
	cout << "Write bin: ";
	cin >> bin;

	try
	{
		ScientificHistogram bar(min, max, bin);
		bar.Create(file);
		bar.Print(cout);
	}
	catch (const Histogram_Exception& exeption)
	{
		cout << exeption.what() << endl;
	}

	system("pause");
	return 0;
}