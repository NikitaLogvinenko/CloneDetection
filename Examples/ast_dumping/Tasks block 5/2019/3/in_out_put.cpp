#include <sstream>
#include <iterator>
#include <cstdlib>
#include <iomanip>
#include <time.h>
#include "Dijkstra_algorithm.h"
#include"friends of friends.h"
#include"in_out_put.h"
using namespace std;
namespace {
	tm create_time(const string& temp) {
		tm time = {};
		istringstream in(temp);
		in >> get_time(&time, "%Y-%m-%dT%X");
		return time;
	}
	news_of_friend create_news(const vector<string>& temp) {
		if (temp.size()< 3)return news_of_friend();
		tm time = create_time(temp.at(0));
		vector<string> current_news;
		for (size_t i = 2; i < temp.size(); i++)
			current_news.push_back(temp.at(i));
		return make_pair(temp.at(1), make_pair(time, current_news));
	}
	void print_time(ostream &os, const tm time) {
		os << put_time(&time,"%Y-%m-%dT%X" )<< " ";
	}
	size_t max__length(const vector<news_of_close_friends> &current_news)
	{
		size_t  max__length = 0;
		for (const auto&[way, friend_news] : current_news) {
				if (friend_news.first.size() > max__length)
					max__length = friend_news.first.size();
		}
		return max__length;
	}
}
vector<weight_edge>read_friends(istream & input) {
	if (!input) return vector<weight_edge>();
	string str;
	vector<weight_edge> graph_friends;
	while (getline(input, str)) {
		vector <string> temp;
		istringstream ist(str);
		copy(istream_iterator<string>(ist), {}, back_inserter(temp));
		for (size_t i =1; i < temp.size(); i++) {
			graph_friends.push_back(make_pair(1, make_pair(temp.at(0), temp.at(i))));
		}
		str.clear();
	}
	return graph_friends;
}
vector<news_of_friend> read_news(istream & input) {
	if (!input) return vector<news_of_friend>();
	string str;
	vector<news_of_friend> news;
	while (getline(input, str)) {
		vector <string> temp;
		istringstream ist(str);
		copy(istream_iterator<string>(ist), {}, back_inserter(temp));
		news.push_back(create_news(temp));
		str.clear();
	}
	return news;
}
void  write_news(ostream &os, const  vector<news_of_close_friends>& current_news) {
	if (!os) return;
	for (const auto& [way,friend_news]:current_news) {
		size_t max_length = max__length(current_news);
		os << setw(max_length +1)<<friend_news.first << "  ";
		print_time(os, friend_news.second.first);
		copy(friend_news.second.second.begin(), friend_news.second.second.end(), ostream_iterator<string>(os, " "));
		os << endl;
	}
}