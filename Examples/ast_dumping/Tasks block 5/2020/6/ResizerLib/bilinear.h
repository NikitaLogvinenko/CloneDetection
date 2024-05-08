#pragma once

#include <cmath>
#include "interpoltype.h"

class Bilinear final: public InterpolationType
{
public:
	double getweight(double x) const override
	{
		double fabsx = fabs(x);
		return fabsx < 1.0 ? 1.0 - fabsx : 0.0;
	}
};