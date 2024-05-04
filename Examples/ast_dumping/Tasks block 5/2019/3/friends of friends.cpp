#pragma once
#include <iostream>
#include <utility>
#include <map>
#include <string>
#include<vector>
#include <sstream>
#include <iterator>
#include<algorithm>
#include <cstdlib>
#include<ctime>
#include"friends of friends.h"
#include"in_out_put.h"
using namespace std;
namespace{
	bool compare_news_by_time(news_of_friend&a, news_of_friend&b) {
		return difftime(mktime(&a.second.first), mktime(&b.second.first)) > 0.0 ? true :false;
	}
	bool compare_news_by_distance(news_of_close_friends &a, news_of_close_friends&b) {
		return a.first < b.first;
	}
	size_t check_friend(const userID &cur_friend, const vector<short_way>& cl_friends) {
		for (const auto& [distance,friends]:cl_friends) {
			userID finded_friend = friends.at(friends.size() - 1);
			if (cur_friend == finded_friend)
				return distance;
		}
		return numeric_limits<int>::max();
	}
	vector <news_of_close_friends>  close_friend_news(const vector<news_of_friend> &current_news, const vector<short_way>& cl_friends) {
		vector<news_of_close_friends> news_cl_friends;
		for (const auto& it:current_news) {
			size_t number = check_friend(it.first, cl_friends);
			news_cl_friends.push_back(make_pair(number, it));
		}
		sort(news_cl_friends.begin(), news_cl_friends.end(), compare_news_by_distance);
		while (news_cl_friends.at(news_cl_friends.size() - 1).first == numeric_limits<int>::max()) {
			news_cl_friends.pop_back();
		}
		return news_cl_friends;
	}
}

vector<news_of_close_friends> NEWS_OF_FRIENDS:: news_of_friends(const userID& source, const  vector<weight_edge>&graph_friends, vector<news_of_friend> &current_news )
{
	if (source.size() == 0 || graph_friends.size() == 0 || current_news.size() == 0) return vector<news_of_close_friends>();
	vector<short_way> cl_friends = Dijkstra_algorithm::find_shortest_way(graph_friends,source);
	sort(current_news.begin(), current_news.end(), compare_news_by_time);
	return close_friend_news(current_news,cl_friends);
}