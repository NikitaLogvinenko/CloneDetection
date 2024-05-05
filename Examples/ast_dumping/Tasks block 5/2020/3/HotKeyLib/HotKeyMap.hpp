#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class HotkeyMap final
{
	std::unordered_map<std::string, std::string> key_cmd_Map_;
	static const std::string NOT_EXIST;
public:

	const std::string& operator[](const std::string& key) const;

	bool add_pair(const std::string& key, const std::string& cmd);

	size_t add_key_list(const std::string& cmd, const std::vector<std::string>& keys);

	bool remove_key(const std::string& key);
	bool remove_cmd(const std::string& cmd);
	void remove_list(const std::vector<std::string>& rlist);

	std::vector<std::string> get_cmds() const;
	std::vector<std::string> get_keys(const std::string& cmd) const;

	HotkeyMap copy() const;
	std::vector<std::string> merge(const HotkeyMap& map);

	void clean();
	bool empty() const;

	friend std::ostream& operator<<(std::ostream& out, const HotkeyMap& map);
};
