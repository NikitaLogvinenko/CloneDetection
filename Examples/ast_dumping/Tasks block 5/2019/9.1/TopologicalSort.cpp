#include "TopologicalSort.h"
#include <algorithm>
#include <iterator>
#include <sstream>
#include <exception>

class CycleException : public std::exception {};

TopologicalSort::TopologicalSort(std::istream & f){
	std::string str, name;		
	while (std::getline(f, str)){
		if (str.empty())
			continue;
		std::istringstream iss(str);
		std::getline(iss, name, '=');
		name.erase(name.size()-1, 1);
		std::vector<std::string> dep;		
		while (getline(iss, str, ',')){
			str.erase(0, 1);
			dep.push_back(str);
		}
		insert(name, dep);
	}
}

void TopologicalSort::insert(const std::string &name, const std::vector<std::string> &dependences)
{
	if (dependences.empty()){
		graph[name];
		return;
	}
	std::copy(dependences.begin(), dependences.end(), std::back_inserter(graph[name]));
	for (int i = 0; i!=dependences.size(); i++)	
		graph[dependences[i]];
}

void TopologicalSort::dfs(std::map<std::string, Color>::iterator index, std::map<std::string, Color> &used, 
	std::vector<std::string> &result, std::map<std::string, std::vector<std::string>>::const_iterator it) const{
	index->second = Color::GREY;
	for (int i = 0; i != it->second.size(); i++){
		std::string cur = it->second[i];
		if (used[cur] == Color::GREY) 
			throw CycleException();
		if (used[cur]==Color::WHITE)
			dfs(used.find(cur), used, result, graph.find(cur));
	}
	index->second = Color::BLACK;
	result.push_back(index->first);
}

std::vector<std::string> TopologicalSort::sort() const{
	std::map<std::string, Color> used;
	std::vector<std::string> result;
	for (auto i = graph.begin(); i != graph.end(); i++)
		used.emplace(i->first, Color::WHITE);
	for (auto i = graph.begin(); i != graph.end(); i++) {
		std::map<std::string, Color>::iterator index = used.find(i->first);
		if (index->second==Color::WHITE) {
			try { dfs(index, used, result, i); }
			catch(CycleException &) {
				result.clear();
				return result;
			}
		}
	}
	return result;
}

