#include "graph_istream_parser.h"


#include <algorithm>
#include <set>
#include <sstream>
#include <cctype>
using namespace std;

namespace
{
	istream& get_word(istream& is, string& str, const set<char>& delimiters)
	{
		char buff = 0;

		is >> skipws >> buff;

		if (!is.eof() && delimiters.find(buff) == delimiters.end())
		{
			is.putback(buff);
		}
		else
		{
			return  is;
		}

		while (is >> noskipws >> buff && delimiters.find(buff) == delimiters.end() && !isspace(buff))
		{
			str.push_back(buff);
		}

		if (isspace(buff))
		{
			if (is >> skipws >> buff && delimiters.find(buff) == delimiters.end())
			{
				is.putback(buff);
			}
		}

		if (is >> skipws >> buff)
		{
			is.putback(buff);
		}

		return is;
	}

	vector<string> get_words(istream& is, const set<char>& delimiters)
	{
		vector<string> words;
		
		while (!is.eof())
		{
			string temp;
			get_word(is, temp, delimiters);
			words.emplace_back(temp);
		}
		return words;
	}

	void to_lower(char& symb)
	{
		symb = static_cast<char>(tolower(symb));
	}
}

bool graph_istream_parser::try_get_graph(graph& graph)
{
	if (!is_.good())
	{
		return false;
	}

	string line;
	skipws(is_);
	while (is_.good() && getline(is_, line))
	{
		if (line.empty())
		{
			continue;
		}

		istringstream iss(line);
		auto words = get_words(iss, delimiters_);

		if (words.size() == 3)
		{
			weight edge_weight = 0;
			try {
				edge_weight = round(stod(words[2]));
			}
			catch (invalid_argument&) {
				return false;
			}

			graph.add_edge(words[0], words[1], edge_weight);
		}
		else
		{
			return false;
		}
	}
	noskipws(is_);
	return true;
}

bool graph_istream_parser::try_get_start_vertex(vertex& start)
{
	if (!is_.good())
	{
		return false;
	}

	string line;

	while (line.empty() && !is_.eof())
	{
		skipws(is_);
		getline(is_, line);
	}
	
	istringstream iss(line);
	
	auto words = get_words(iss, delimiters_);

	if (words.size() == 2)
	{
		for_each(words[0].begin(), words[0].end(), to_lower);

		if (words[0] == "start" || words[0] == "s")
		{
			start = words[1];
			return true;
		}
	}
	
	return false;
}