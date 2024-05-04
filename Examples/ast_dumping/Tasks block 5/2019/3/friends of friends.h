#pragma once
#include <iostream>
#include <string>
#include<vector>
#include <ctime>
#include"Dijkstra_algorithm.h"
typedef std::string userID;
typedef std::pair<std::tm , std::vector<std::string >> news;
typedef std::pair < userID, news> news_of_friend;
typedef std::pair<int, news_of_friend> news_of_close_friends;
class  NEWS_OF_FRIENDS {
public:
	 static std::vector<news_of_close_friends> news_of_friends(const userID& source,const std::vector<weight_edge>&graph_friends, std::vector<news_of_friend>& current_news);
};