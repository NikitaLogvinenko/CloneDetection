#include <iostream>
#include <sstream>
#include <map>
#include <ctime>
#include <vector>
#include "obeder.hpp"


void Obeder::create_obed(std::istream &_is) {
	Parser parc(_is);
	obed = parc.getData();
}

std::multimap<double, std::string> depts(std::multimap<time_t, std::pair<std::string, double>> &obed, time_t t_start, time_t t_end) {
	auto my_comp = obed.key_comp();
	auto it = obed.lower_bound(t_start);
	double sum = 0;
	int count = 0;
	auto start_it = it;
	while (it!=obed.end()) {
		const auto&[name, cash] = it->second;
		if (!my_comp(t_end, (*it).first)) {
			count++;
			sum += cash;
			*it++;
		}
	}
	double dept = sum / count;
	std::map<std::string, double> depts_list;
	auto end_it = it;
	it = start_it;
	auto it_dept = depts_list.begin();
	while (it != end_it) {
		auto&[name, cash] = it->second;
		it_dept = depts_list.find(name);
		if (it_dept == depts_list.end()) {
			cash -= dept;
			depts_list.insert((*it).second);
		}
		else {
			(*it_dept).second += cash;
		}
		*it++;
	}

	std::multimap<double, std::string> cash_list;
	it_dept = depts_list.begin();
	while (it_dept != depts_list.end()) {
		const auto&[cash, name] = *it_dept;
		cash_list.insert(std::make_pair(name, cash)); 
		*it_dept++;
	}
	return cash_list;
}

std::vector<std::tuple<std::string, std::string, double>> Obeder::vect_depts(time_t t_start, time_t t_end) {
	std::multimap<double, std::string> cash = depts(obed, t_start, t_end);
	std::vector<std::tuple<std::string, std::string, double>> dept;
	while (cash.size()>1) {
		auto it_begin = cash.begin();
		auto it_last = --cash.end();
		//double cash1 = it_begin->first;
	//	double cash2 = it_last->first;
		const auto&[sum1, name1] = *it_begin;
		const auto&[sum2, name2] = *it_last;
		if (sum1 != 0 && sum2 != 0) {
			if (-sum1 > sum2) {
				dept.push_back(std::make_tuple(it_begin->second, it_last->second, abs(sum2)));
				cash.insert(std::make_pair(sum2 + sum1, name1));
			}
			else {
				dept.push_back(std::make_tuple(it_begin->second, it_last->second, abs(sum1)));
				cash.insert(std::make_pair(sum2 + sum1, name2));
			
			}
		}
		cash.erase(it_begin);
		cash.erase(it_last);
	}
	return dept;
}


void output_dept(const std::vector<std::tuple<std::string, std::string, double>> &depts, std::ostream &outs) {
	auto it_vec = depts.begin();
	//auto it_par = it_vec->first;
	while (it_vec!=depts.end()) {
		//it_par = it_vec->first;
		outs << std::get<0>(*it_vec)<< "->" << std::get<1>(*it_vec) << " " << std::get<2>(*it_vec) << std::endl;
		*it_vec++;
	}
}



std::multimap<time_t, std::pair<std::string, double>> Parser::getData() {
	std::multimap<time_t, std::pair<std::string, double>> obed;
	std::string line;

	while (std::getline(is, line)) {
		if (line.size() == 0) break;
		std::string temp_line;
		time_t time;
		std::string name;
		double cash;
		std::istringstream iss(line);
		std::getline(iss, temp_line, ' ');

		time = (time_t)atoi(temp_line.c_str());
		std::getline(iss, temp_line, ' ');
		name = temp_line;
		std::getline(iss, temp_line, ' ');
		cash = std::stod(temp_line);

		std::pair<std::string, double> t_pair(name, cash);
		obed.insert(std::make_pair(time, t_pair));
	}
	return obed;
}


void Obeder::add_obed(time_t time, std::string & name, double cash) {
	auto temp = std::make_pair(name, cash);
	obed.insert(std::make_pair(time, temp));
}

void Obeder::delete_obed(time_t t_start, time_t t_end) {
	auto it = obed.begin();
	while ((*it).first < t_start && it!=obed.end())
		*it++;
	while ((*it).first < t_end && it != obed.end()) {
		obed.erase(it);
		*it++;
	}
}