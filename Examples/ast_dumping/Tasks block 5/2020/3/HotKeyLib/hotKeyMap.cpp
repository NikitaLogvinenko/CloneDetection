#include <algorithm>
#include <ostream>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include "HotKeyMap.hpp"

const std::string HotkeyMap::NOT_EXIST;


const std::string& HotkeyMap::operator[](const std::string& key) const
{
	const auto it = key_cmd_Map_.find(key);

	if (it == key_cmd_Map_.end())
	{
		return NOT_EXIST;
	}
	
	return it->second;
}


bool HotkeyMap::add_pair(const std::string& key, const std::string& cmd)
{
	const auto newElement = key_cmd_Map_.insert(std::make_pair(key, cmd));
	return newElement.second;
}


size_t HotkeyMap::add_key_list(const std::string& cmd, const std::vector<std::string>& keys)
{
	size_t curr_pos = 0;
	for (auto& key : keys)
	{
		++curr_pos;
		if (!add_pair(key, cmd))
		{
			return curr_pos - 1;
		}
	}
	return std::numeric_limits<size_t>::max();
}


bool HotkeyMap::remove_key(const std::string& key)
{
	const auto it = key_cmd_Map_.find(key);

	if (it == key_cmd_Map_.end())
	{
		return false;
	}
	
	key_cmd_Map_.erase(it);
	return true;
}


bool HotkeyMap::remove_cmd(const std::string& cmd)
{
	bool elemExist = false;

	for (auto it = key_cmd_Map_.begin(); it != key_cmd_Map_.end();)
	{
		if (it->second == cmd)
		{
			it = key_cmd_Map_.erase(it);
			elemExist = true;
		}
		else
		{
			++it;
		}
	}
	return elemExist;
}

void HotkeyMap::remove_list(const std::vector<std::string>& rlist)
{
	for (const auto& it : rlist)
	{
		remove_cmd(it);
		remove_key(it);
	}
}



std::vector<std::string> HotkeyMap::get_cmds() const
{
	std::vector<std::string> cmds;
	std::unordered_set<std::string> uniqueCmd;

	for (const auto& it : key_cmd_Map_)
	{
		uniqueCmd.insert(it.second);
	}

	cmds.assign(uniqueCmd.begin(), uniqueCmd.end());
	
	return cmds;
}


std::vector<std::string> HotkeyMap::get_keys(const std::string& cmd) const
{

	std::vector<std::string> keys;
	
	auto extract_keys = [&cmd, &keys](const std::pair<std::string, std::string>& it)
	{
		if (it.second == cmd)
			keys.push_back(it.first);
	};

	std::for_each(key_cmd_Map_.begin(), key_cmd_Map_.end(), extract_keys);
	
	return keys;
}


HotkeyMap HotkeyMap::copy() const
{
	HotkeyMap copyHkMap;

	auto copy = [&copyHkMap](const std::pair<std::string, std::string>& it)
	{
		copyHkMap.add_pair(it.first, it.second);
	};
	
	std::for_each(key_cmd_Map_.begin(), key_cmd_Map_.end(), copy);
	return copyHkMap;
}


std::vector<std::string> HotkeyMap::merge(const HotkeyMap& map)
{
	std::vector<std::string> conflicts;
	std::vector<std::string> cmds = map.get_cmds();
	
	for (auto& cmd : cmds)
	{
		auto isConflict = [&conflicts, &cmd, this](const std::string& key)
		{
			if (!add_pair(key, cmd) && (*this)[key] != cmd)
				conflicts.push_back(key);
		};
		
		std::vector<std::string> keys = map.get_keys(cmd);
		std::for_each(keys.begin(), keys.end(), isConflict);
	}
	
	return conflicts;
}

bool HotkeyMap::empty() const
{
	return key_cmd_Map_.empty();
}


void HotkeyMap::clean()
{
	key_cmd_Map_.clear();
}


std::ostream& operator<<(std::ostream& out, const HotkeyMap& map)
{
	std::unordered_map<std::string, std::list<std::string>> unordered_map;
	for (const auto& it : map.key_cmd_Map_)
	{
		unordered_map[it.second].push_back(it.first);
	}

	for (const auto& it : unordered_map)
	{
		out << it.first << " ";
		for (const auto& list_it : it.second)
			out << list_it << ", ";
		out << std::endl;
	}
	return out;
}
