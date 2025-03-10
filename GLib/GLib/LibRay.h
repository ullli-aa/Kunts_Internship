#pragma once

#include "LibLine.h"

template<typename T>
class LibRay : public LibLine<T>
{
public:
	LibRay(const LibPoint<T>& ptOrigin, const LibVector<T>& vecDirection)
		: LibLine<T>(ptOrigin, vecDirection) {}

	bool IsIntersectionLine(const LibLine<T>& lnOther, LibPoint<T>& intersPoint) const override
	{
		T coefR, coefL;
		if (this->GetIntersParam(lnOther, coefR, coefL))
		{
			if (coefR < 0)
			{
				return false;
			}
			LibPoint<T> intersPoint;
			return this->GetIntersection(coefR, intersPoint);
		}
		return false;
	};

	bool IsIntersectionRay(const LibRay<T>& lnOther, LibPoint<T>& intersPoint) const
	{
		T coef1, coef2;
		if (GetIntersParam(lnOther, coef1, coef2))
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
		double projectionLength = this->GetCoefClosestPoint(point);
		if (projectionLength < 0)
		{
			return this->Origin();
		}
		return this->GetClosestPoint(projectionLength);
	};

	bool IsPointOnLine(const LibPoint<T>& point, double eps = LibEps::eps) const override
	{
		LibVector<T> vec = LibVector<T>(point - this->Origin());
		return !vec.IsOpposite(this->Direction()) && vec.LengthVector() >= 0;
	};
};
