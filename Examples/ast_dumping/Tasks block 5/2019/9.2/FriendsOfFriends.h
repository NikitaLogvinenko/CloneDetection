#pragma once
#include <map>
#include <vector>
//#include <iostream>
#include <istream>
#include <string>
#include <ctime>
//#include <utility>

class FriendsOfFriends {
	std::map<std::string, std::vector<std::string>> friends;
	std::map<std::string, std::map<time_t, std::string>> feeds;
	void find_n_add_posts(const std::string &userid, std::multimap<time_t, std::pair<std::string, std::string>> &notes) const;
public:
	FriendsOfFriends() = default;
	FriendsOfFriends(std::istream &friends, std::istream &feeds);
	void insert_friends(const std::string &user, const std::vector<std::string> &friends);
	void insert_friend(const std::string &user, const std::string &new_friend);
	void parse_friends(std::istream &is);
	void parse_feeds(std::istream &is);
	void insert_feeds(const std::vector<std::string> &feeds);
	std::multimap<time_t, std::pair<std::string, std::string>> user_feeds(const std::string &userid) const;
	std::multimap<time_t, std::pair<std::string, std::string>> friend_feeds(const std::string &userid) const;
};
