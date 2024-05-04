#include "event.h"

Event::Event(time_t _time, const std::string& _name, int _moneyBill)
{
	time = _time;
	name = _name;
	moneyBill = _moneyBill;
}

Event::Event(const time_t _time)
{
	time = _time;
	moneyBill = 0;
}

const time_t& Event::getTime() const
{
	return time;
}

const std::string& Event::getName() const
{
	return name;
}

int Event::getMoneyBill() const
{
	return moneyBill;
}

bool Event::operator==(const Event& x) const
{
	return time == x.time;
}

bool Event::operator<(const Event& x) const
{
	return time < x.time;
}