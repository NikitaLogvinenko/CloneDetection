#pragma once
#include <cmath>
#include <corecrt_math_defines.h>
#include "interpoltype.h"
class Lanczos final: public InterpolationType
{
	
public:

	Lanczos(int _a) :a(_a) {}
	double getweight(double x) const override
	{
		return fabs(x) < a ? sinc(M_PI*x) * sinc(fabs(M_PI*x / a)) : 0;
	}
private:

	double sinc(double f_x) const {
		if (0.0 == f_x) return 1.0;
		return sin(f_x) / (f_x);
	}
	int a;
};