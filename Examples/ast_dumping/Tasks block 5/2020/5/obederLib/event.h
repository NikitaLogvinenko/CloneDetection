#pragma once
#include <ctime>
#include <string>

class Event final
{
private:
	time_t time;
	std::string name;
	int moneyBill;
public:
	Event(time_t _time, const std::string& _name, int _moneyBill);
	Event(time_t _time);
	~Event() = default;
	const time_t& getTime() const;
	const std::string& getName() const;
	int getMoneyBill() const;
	bool operator==(const Event& x) const;
	bool operator<(const Event& x) const;
};
