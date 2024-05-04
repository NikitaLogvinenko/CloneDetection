#include "../graph/Dijkstra.h"

#include <iostream>
#include <fstream>

enum exit_params
{
	success = 0,
	invalid_argument_count,
	failed_loading_file,
	algorithm_error,
	failed_saving_file
};

using namespace std;

int main(const int argc, char** argv)
{
	string program_name(argv[0]);
	program_name = program_name.substr(program_name.find_last_of('\\') + 1);
	
	if (argc < 2 || argc >3) 
	{
		std::cout << "Invalid arguments. Use this argument struct:\n"
			<< "\t" << program_name << "[input_filename]\tor\n"
			<< "\t" << program_name << "[input_filename] [output_filename]\n";

		return invalid_argument_count;
	}

	const string input_filename(argv[1]);
	string output_filename;

	if (argc == 2) 
	{
		const auto dot_pos = input_filename.find_first_of('.');
		output_filename.append(input_filename.substr(0, dot_pos) + " - output.txt");
	}
	else
		output_filename.append(argv[2]);

	fstream input_file(input_filename, ios::in);
	if (!input_file.is_open()) 
	{
		cout << "Couldn't open input file." << endl;
		return failed_loading_file;
	}

	fstream output_file(output_filename, ios::out);
	if (!output_file.is_open())
	{
		cout << "Couldn't create output file" << endl;
		return failed_saving_file;
	}

	dijkstra dijkstra_graph;

	if (!dijkstra_graph.load_graph(input_file))
	{
		cout << "Incorrect input file\n";
		return failed_loading_file;
	}

	input_file.close();
	try
	{
		dijkstra_graph.build();
	}
	catch (out_of_range& ec)
	{
		cout << "Algorithm error. " << ec.what() << endl;
		return algorithm_error;
	}

	if (!dijkstra_graph.save_result(output_file))
	{
		cout << "An error occurred while saving the file." << endl;
		return failed_saving_file;
	}
	
	output_file.close();

	cout << "File saved successfully: " + output_filename << endl;
	return success;
}
