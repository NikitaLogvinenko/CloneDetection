#pragma once

class InterpolationCurve
{
public:
	InterpolationCurve() = default;
	virtual double getWeight(double x) const = 0;
	
	virtual ~InterpolationCurve() = default;

};