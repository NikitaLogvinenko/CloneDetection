#include "hotkey.hpp"
#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <vector>

HotkeyMap::HotkeyMap(const std::multimap<std::string, std::string> & keys) : key_map(keys) {}

HotkeyMap::HotkeyMap() {}

HotkeyMap::HotkeyMap(std::istream &_is) {
	Parser inp(_is);
	std::pair<std::string, std::string> indata;
	indata = inp.getData();
	while (indata.first != "") {
		key_map.insert(indata);
		indata = inp.getData();
	}
}

const std::string HotkeyMap::operator[](const std::string & key) const {
	auto it = key_map.find(key);
	if (it == key_map.end()) {
		return "";//std::string::npos;
	}
	return it->second;
}

bool HotkeyMap::add_key(const std::string & key, const std::string & cmd) {
	auto it = key_map.find(key);
	if (it != key_map.end()) {
		while (it->first == key) {
			if (it->second == cmd)
				return false;
		}
	}
	key_map.insert(std::make_pair(key, cmd));
	return true;
}

bool HotkeyMap::add_cmd(const std::string & cmd, std::vector < std::string > keys) {
	auto it_v = keys.begin();
	std::string tmp_str;
	if (it_v != keys.end())
		tmp_str += *it_v;
	*it_v++;
	while (it_v != keys.end()) {
		tmp_str += '+' + *it_v;
		*it_v++;
	}
	auto it = key_map.find(tmp_str);
	if (it != key_map.end()) {
		while (tmp_str.compare(it->first) == 0) {
			if (cmd.compare(it->second) == 0)
				return false;
		}
	}
	key_map.insert(std::make_pair(tmp_str, cmd));
	return true;
}


bool HotkeyMap::remove_key(const std::string & key) {
	auto it = key_map.find(key);
	if ( it == key_map.end()) {
		return false;
	}
	key_map.erase(it);
	return true; 
}

bool HotkeyMap::remove_cmd(const std::string & cmd) {
	for (auto  it = key_map.begin(); it != key_map.end(); *it++) {
		auto &[key, k_cmd] = *it ;
		if (k_cmd == cmd) {
			key_map.erase(it);
			return true;
		}
	}
	return false;
}


std::set<std::string> HotkeyMap::get_cmds(void) const { 
	std::set<std::string> cmd_set;
	for (auto&[keys, cmd] : key_map) {
		cmd_set.insert(cmd);
	}
	return cmd_set;
}

std::vector<std::string> HotkeyMap::get_keys(const std::string & cmd) const { 
	std::vector<std::string> cmd_name;
	for (auto&[keys, k_cmd] : key_map) {
		if (k_cmd == cmd)
			cmd_name.push_back(keys);
	}
	return cmd_name;
}


HotkeyMap HotkeyMap::copy(void) const { 
	return *this;
}

std::vector<std::string> HotkeyMap::merge(const HotkeyMap & map) {
	std::vector<std::string> confl;
	for (auto it = map.key_map.begin(); it != map.key_map.end(); *it++) {
		if (key_map.find(it->first) != key_map.end())
			confl.push_back(it->first);
		key_map.insert(*it);
	}
	return confl;
}

void HotkeyMap::clean(void) {
	key_map.clear();
}



std::pair<std::string, std::string> Parser::getData() {
	std::string line;

	while (std::getline(is, line)) {
		if (line.size() == 0) break;
		std::string temp_line;
		std::string keycom;
		std::string action;

		std::istringstream iss(line);

		std::getline(iss, temp_line, '=');
		keycom = temp_line;
		std::getline(iss, temp_line, ' ');
		action = temp_line;
		return std::make_pair(keycom, action);
	}
	return std::make_pair("", "");
}

