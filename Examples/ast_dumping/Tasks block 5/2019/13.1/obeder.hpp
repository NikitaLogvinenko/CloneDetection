#pragma once
#include <iostream>
#include <sstream>
#include <map>
#include <ctime>


class Obeder {
	std::multimap<time_t, std::pair<std::string, double>> obed;
	std::vector<std::tuple<std::string, std::string, double>> dept;
public:
	void create_obed(std::istream &_is);
	std::vector<std::tuple<std::string, std::string, double>> vect_depts(time_t t_start, time_t t_end);
	void add_obed(time_t time, std::string& name, double cash);
	void delete_obed(time_t t_start, time_t end);
};



class Parser {
private:
	std::istream &is;
public:
	Parser(std::istream &_is) : is(_is) {}

	std::multimap<time_t, std::pair<std::string, double>> getData();
};

void output_dept(const std::vector<std::tuple<std::string, std::string, double>> &depts, std::ostream &outs);