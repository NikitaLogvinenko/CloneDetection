#include "payout.h"

Payout::Payout(const Person* _from, const Person* _to, double _moneyBill)
{
	from = _from;
	to = _to;
	moneyBill = _moneyBill;
}

Payout::Payout(const Person* _from)
{
	from = _from;
	to = nullptr;
	moneyBill = 0;
}

const Person& Payout::getFrom() const
{
	return *from;
}

const Person& Payout::getTo() const
{
	return *to;
}

double Payout::getMoneyBill() const
{
	return moneyBill;
}

void Payout::amountMoneyBill(double money) const
{
	moneyBill += money;
}

void Payout::setMoneyBill(double money) const
{
	moneyBill = money;
}

bool Payout::operator<(const Payout& a) const
{
	return from->getName() < a.getFrom().getName();
}