#pragma once
#include <iostream>
#include <string>
#include<vector>
#include "Dijkstra_algorithm.h"
#include"friends of friends.h"

void  write_news(std::ostream &os, const std::vector<news_of_close_friends>& current_news);
std::vector<weight_edge> read_friends(std::istream & input);
std::vector <news_of_friend> read_news(std::istream & input);

