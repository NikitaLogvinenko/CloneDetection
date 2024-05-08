#include "obeder.h"


#include <algorithm>
#include <string>

DinnerHistory::DinnerHistory(const std::set<Event>& _events)
{
	events = _events;
	for (const auto& element : events)
	{
		persons.insert(Person(element.getName()));
	}
}

void DinnerHistory::eventAdd(const Event& _event)
{
	events.insert(_event);
	persons.insert(Person(_event.getName()));
}

std::set<Payout> DinnerHistory::setUpPayouts() const
{
	std::set<Payout> payouts;
	for (const auto& element : persons)
	{
		payouts.insert(Payout(&element,
			persons.upper_bound(element) == persons.end() ? &*persons.begin() : &*persons.upper_bound(element), 0));
	}
	return payouts;
}

void DinnerHistory::generateMoneyBill(std::set<Payout>& payouts, const timeInterval range) const
{
	auto left = events.lower_bound(Event(range.first));
	auto right = events.upper_bound(Event(range.second));
	double sumBill = 0;
	for (; left != right; left = events.upper_bound(*left))
	{
		const Person person(left->getName());
		auto it = payouts.find(Payout(&person));
		it->amountMoneyBill(left->getMoneyBill());
		sumBill += left->getMoneyBill();
	}
	sumBill = sumBill / persons.size();
	double mathBuffer = 0;
	for (const auto& element : payouts)
	{
		element.setMoneyBill(mathBuffer += sumBill - element.getMoneyBill());
	}
}

void DinnerHistory::shiftMoneyBill(std::set<Payout>& payouts) const
{
	const auto minPayout = std::min_element(payouts.begin(), payouts.end());
	const double minBill = minPayout->getMoneyBill();
	for (auto value : payouts)
	{
		value.amountMoneyBill(-minBill);
	}
}

std::set<Payout> DinnerHistory::makePayouts(const timeInterval range) const
{
	std::set<Payout> payouts = setUpPayouts();
	generateMoneyBill(payouts, range);
	shiftMoneyBill(payouts);
	return payouts;
}

