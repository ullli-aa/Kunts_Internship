#pragma once

#include "LibLine.h"
#include <variant>

template<typename T>
class LibRay : public LibLine<T>
{
public:
	bool IsIntersectionLine(const LibLine<T>& lnOther, LibPoint<T>& intersPoint) const override
	{
		T coefR, coefL;
		if (GetIntersParam(lnOther, coefR, coefL))
		{
			if (coefR < 0)
			{
				return false;
			}
			return GetIntersection(coefR);
		}
		return false;
	};

	bool IsIntersectionRay(const LibRay<T>& lnOther, LibPoint<T>& intersPoint) const
	{
		T coef1, coef2;
		if (GetIntersParam(lnOther, coefR, coefL))
		{
			if (coef1 >= 0 && coef2 >= 0)
			{
				return GetIntersection(coef2);
			}
		}
		return false;
	};

	LibPoint<T> ClosestPointOnLine(const LibPoint<T>& point) const override
	{
		double projectionLength = GetCoefClosestPoint(point);
		if (projectionLength < 0)
		{
			return this->Origin();
		}
		return GetClosestPoint(coef);
	};

	bool IsPointOnLine(const LibPoint<T>& point) const override
	{
		LibVector<T> vec = LibVector<T>(point - this->Origin());
		return !vec.IsOpposite(this->Direction()) && vec.LengthVector() >= 0;
	};
};
