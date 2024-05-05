#pragma once
#include <string>
#include <vector>
#include <set>
#include <map>
#include <iostream>


class HotkeyMap {
	std::multimap<std::string, std::string> key_map; //keys, action
	
public:
	HotkeyMap(const std::multimap<std::string, std::string> & keys);
	HotkeyMap(std::istream &_is);

	HotkeyMap();

	const std::string operator[](const std::string & key) const;

	bool add_key(const std::string & key, const std::string & cmd);

	bool add_cmd(const std::string & cmd, std::vector < std::string > keys);

	bool remove_key(const std::string & key);
	bool remove_cmd(const std::string & cmd);

	std::set<std::string> get_cmds(void) const;
	std::vector<std::string> get_keys(const std::string & cmd) const;

	HotkeyMap copy(void) const;
	std::vector<std::string> merge(const HotkeyMap & map);
	void clean(void);
};

class Parser {
private:
	std::istream &is;
public:
	Parser(std::istream &_is) : is(_is) {}

	std::pair<std::string, std::string> getData();
};