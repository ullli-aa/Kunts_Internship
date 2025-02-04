#pragma once

class LibEps
{
public:
	static double eps;

	bool IsZero(double numb, double otherEps = eps)
	{
		return numb <= otherEps;
	}
};
