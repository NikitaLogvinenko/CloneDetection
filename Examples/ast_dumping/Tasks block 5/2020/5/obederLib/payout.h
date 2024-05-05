#pragma once
#include "person.h"

class Payout final
{
private:
	const Person* from;
	const Person* to;
	mutable double moneyBill;
public:
	Payout() = delete;
	explicit Payout(const Person* _from, const Person* _to, double _moneyBill);
	Payout(const Person* _from);
	~Payout() = default;
	const Person& getFrom() const;
	const Person& getTo() const;
	double getMoneyBill() const;
	void amountMoneyBill(double money) const;
	void setMoneyBill(double money) const;
	bool operator<(const Payout& a) const;
};