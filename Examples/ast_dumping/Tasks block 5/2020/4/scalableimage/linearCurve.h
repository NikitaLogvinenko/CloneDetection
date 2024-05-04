#pragma once

#include <cmath>
#include "curves.h"

class LinearCurve final : public InterpolationCurve
{
public:
	double getWeight(double x) const override
	{
		return abs(x) < 1.0 ? 1.0 - abs(x) : 0.0;
	}
};
