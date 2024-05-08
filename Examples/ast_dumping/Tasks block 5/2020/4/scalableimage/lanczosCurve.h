#pragma once
#include <math.h>
#include <corecrt_math_defines.h>

#include "curves.h"
class LanczosCurve final : public InterpolationCurve
{
public:
	int a;
	LanczosCurve(int _a) :a(_a) {}
	LanczosCurve() :a(2) {}


	double getWeight(double x) const override
	{
		return abs(x) == 0 ? 1 : abs(x) < a ? sinc(M_PI * abs(x)) * sinc(M_PI * abs(x / a)) : 0;
	}
private:
	double sinc(double x) const
	{
		return sin(x) / x;
	}
};
