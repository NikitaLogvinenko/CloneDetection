#pragma once
class InterpolationType
{
public:
	InterpolationType() = default;
	virtual double getweight(double x) const = 0;
	virtual ~InterpolationType() = default;
};