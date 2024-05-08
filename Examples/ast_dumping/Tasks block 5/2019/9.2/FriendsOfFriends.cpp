#include"FriendsOfFriends.h"
#include <algorithm>
#include <iterator>
#include <iomanip>
#include<sstream>

void FriendsOfFriends::find_n_add_posts(const std::string &userid, std::multimap<time_t, std::pair<std::string, std::string>> &notes) const{
	auto iter = friends.find(userid);
	for (int i = 0; i != iter->second.size(); i++) {
		auto post = feeds.find(iter->second[i]);
		if (post == feeds.end())
			continue;
		for (auto it = post->second.begin(); it != post->second.end(); it++) {
			std::pair <std::multimap<time_t, std::pair<std::string, std::string>>::iterator,
				std::multimap<time_t, std::pair<std::string, std::string>>::iterator> ret = notes.equal_range(it->first);
			int count = 0;
			for (auto itr = ret.first; itr != ret.second; itr++) {
				if ((std::get<0>(itr->second).compare(iter->second[i]) == 0) && (std::get<1>(itr->second).compare(it->second) == 0))
					count++;
			}
			if (count != 0)
				continue;
			std::pair<std::string, std::string> p(iter->second[i], it->second);
			notes.emplace(it->first, p);
		}
	}
}

std::multimap<time_t, std::pair<std::string, std::string>> FriendsOfFriends::friend_feeds(const std::string &userid) const {
	std::multimap<time_t, std::pair<std::string, std::string>> notes;
	auto iter = friends.find(userid);
	if (iter == friends.end())
		return notes;
	find_n_add_posts(userid, notes);
	for (int i = 0; i != iter->second.size(); i++)
		this->find_n_add_posts(iter->second[i], notes);
	return notes;
}

std::multimap<time_t, std::pair<std::string, std::string>> FriendsOfFriends::user_feeds(const std::string &userid) const { ///username???
	std::multimap<time_t, std::pair<std::string, std::string>> notes;
	auto iter = friends.find(userid);
	if (iter == friends.end())
		return notes;
	find_n_add_posts(userid, notes);
	return notes;
}

void FriendsOfFriends::parse_friends(std::istream &is) {
	std::string str;
	while (std::getline(is, str)) {
		if (str.empty())
			continue;
		std::vector<std::string> v;
		std::istringstream isstr(str);
		std::copy(std::istream_iterator<std::string>(isstr), {}, std::back_inserter(v));
		std::string user = v[0];
		v.erase(v.begin());
		this->insert_friends(user, v);
	}
}

void FriendsOfFriends::parse_feeds(std::istream &is) {
	std::string str;
	while (std::getline(is, str)) {
		if (str.empty())
			continue;
		std::vector<std::string> v;
		std::istringstream isstr(str);
		std::string time, name;
		isstr >> time >> name;
		v.push_back(time);
		v.push_back(name);
		getline(isstr, str);
		if (str.size() == (time.size()+name.size()+1))
			continue;
		str.erase(0, 1);
		v.push_back(str);
		this->insert_feeds(v);
	}
}

void FriendsOfFriends::insert_friends(const std::string &user, const std::vector<std::string> &friends)
{
	std::copy(friends.begin(), friends.end(), std::back_inserter(this->friends[user]));
	for (int i = 0; i != friends.size(); i++)
		this->friends[friends[i]];
}

void FriendsOfFriends::insert_friend(const std::string &user, const std::string &new_friend) {
	this->friends[user].push_back(new_friend);
	this->friends[new_friend];
}

void FriendsOfFriends::insert_feeds(const std::vector<std::string> &feeds) {
	std::tm tm;
	std::string timestr = feeds[0];
	std::istringstream iss(timestr);
	iss >> std::get_time(&tm, "%Y-%m-%dT%T");
	std::time_t time = mktime(&tm);
	this->feeds[feeds[1]].emplace(time, feeds[2]);
}

FriendsOfFriends::FriendsOfFriends(std::istream &friends, std::istream &feeds) {
	parse_friends(friends);
	parse_feeds(feeds);
}