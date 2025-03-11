#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

class LibEps
{
public:
	static double eps;

	template<typename T>
	static bool IsZero(T numb, double otherEps = 1e-9)
	{
		return fabs(numb) <= otherEps;
	};
};