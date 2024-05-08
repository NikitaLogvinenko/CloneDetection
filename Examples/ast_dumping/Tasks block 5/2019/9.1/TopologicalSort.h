#pragma once
#include <iostream>
#include <istream>
#include <map>
#include <vector>

enum class Color {
	WHITE, GREY, BLACK
};

class TopologicalSort final
{
private:
	std::map<std::string, std::vector<std::string>> graph;
	void dfs(std::map<std::string, Color>::iterator index, std::map<std::string, Color>& used, 
		std::vector<std::string>& result, std::map<std::string, std::vector<std::string>>::const_iterator it) const;
public:
	TopologicalSort() = default;
	TopologicalSort(std::istream &is);
	void insert(const std::string &name, const std::vector<std::string>& dependences);	
	std::vector<std::string> sort() const;
};