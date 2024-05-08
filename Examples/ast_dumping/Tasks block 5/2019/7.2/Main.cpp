#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "Crc16.h"

using namespace std;

int main(int argc, char* argv[])
{
	vector<Crc16> filesaa;
	string command;
	vector<string> commands;
	if (argc == 1)
	{
		cout << "Manifest file generation: '<binary_name> --calc <file_name_1> ... <file_name_N>'" << endl;
		cout << "Check files: '<binary_name> --check <manifest_filename>'" << endl;
		getline(cin, command);
		commands = Crc16::strToWords(command, " ");
	}
	else
		for (int i = 1; i < argc; i++)
			commands[i] = argv[i];

	
	if		(commands[1] == "--calc")
	{
		Crc16::writeCheckToManifest(commands);
	}
	else if (commands[1] == "--check")
	{
		map<string, int> manifest = Crc16::readManifest(commands[2]);
		map<string, int> inputdata = Crc16::readManifest(commands[0]);

		map <string, int> ::iterator it;
		for (it = inputdata.begin(); it != inputdata.end(); it++)
		{
			cout << it->first << " : ";
			if (it->second == manifest[it->first])
				cout << "OK" << endl;
			else
				cout << "FAILED" << endl;
		}

	}
	
	system("pause");
}