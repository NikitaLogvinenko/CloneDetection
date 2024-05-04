#include "friend_adviser.h"

#include <unordered_map>
#include <unordered_set>
#include <map>
#include <iterator>

namespace
{
	const double EPSILON = 1e-5;
	typedef std::unordered_map<std::string, std::vector<std::string>> Users;

	const std::vector<std::string>& get_friends(const std::string& user_id, const Users& users)
	{
		return users.at(user_id);
	}
	std::unordered_set<std::string> get_banned_friends(const std::string& user_id, const std::vector<std::string>& friends)
	{
		std::unordered_set<std::string> result;
		result.insert(user_id);
		for (const auto& i : friends)
		{
			result.insert(i);
		}

		return result;
	}
	std::map<std::string, double> get_fofriends_frequency(const std::string& user_id, const Users& users)
	{
		const std::vector<std::string>& friends = get_friends(user_id, users);
		size_t friends_amount = friends.size();

		std::unordered_set<std::string> ban_list = get_banned_friends(user_id, friends);

		std::map<std::string, double> result;
		for (const auto& i: friends)
		{
			const std::vector<std::string>& fofriends = get_friends(i, users);
			for (const auto& j: fofriends)
			{
				if (ban_list.find(j) == ban_list.end())
				{
					result[j] += 1.0 / friends_amount;
				}
			}
		}

		return result;
	}
	std::vector<std::string> get_suitable_friends(const std::map<std::string, double>& friends_frequency, double threshold)
	{
		std::vector<std::string> result;

		for (const auto& i: friends_frequency)
		{
			if (threshold <= i.second + EPSILON)
			{
				result.push_back(i.first);
			}
		}

		return result;
	}
}

std::vector<std::string> friend_adviser::get_new_friends(const std::string& user_id, double threshold)
{
	Users users;
	Friends names;
	while (prc.get_names(names))
	{
		users[names.first].push_back(names.second);
		users[names.second].push_back(names.first);
	}

	std::vector<std::string> result;
	if (users.empty())
	{
		return result;
	}
	
	std::map<std::string, double> fofriends;
	fofriends = get_fofriends_frequency(user_id, users);
		
	result = get_suitable_friends(fofriends, threshold);

	return result;
}