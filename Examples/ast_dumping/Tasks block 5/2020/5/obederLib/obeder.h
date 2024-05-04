#pragma once
#include <set>

#include "event.h"
#include "payout.h"
#include "person.h"

typedef std::pair<time_t, time_t> timeInterval;

class DinnerHistory final
{
private:
	std::set<Event> events;
	std::set<Person> persons;
	std::set<Payout> setUpPayouts() const;
	void generateMoneyBill(std::set<Payout>& payouts, const timeInterval range) const;
	void shiftMoneyBill(std::set<Payout>& payouts) const;
public:
	DinnerHistory(const std::set<Event>& _events);
	~DinnerHistory() = default;
	void eventAdd(const Event& _event);
	std::set<Payout> makePayouts(const timeInterval range) const;
};


