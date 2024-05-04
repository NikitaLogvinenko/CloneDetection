#pragma once
#include <cmath>
#include "interpoltype.h"

class Bicubic final: public InterpolationType
{
public:
	double getweight(double x) const override
	{
		const double absX = abs(x);
		return absX < 1.0
			? 0.5f * (absX * absX - 1) * (absX - 2)
			: absX < 2
			? -(1.0 / 6.0) * (absX - 1) * (absX - 2) * (absX - 3)
			: 0;
	}
};
