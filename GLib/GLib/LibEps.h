#pragma once

#include <cmath>

class LibEps
{
public:
	static double eps;

	template<typename T>
	static bool IsZero(T numb, double otherEps = 1e-9)
	{
		return std::fabs(numb) <= otherEps;
	};
};