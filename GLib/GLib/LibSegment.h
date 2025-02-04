#pragma once

#include "LibLine.h"
#include "LibRay.h"

template<typename T>
class LibSegment : public LibLine<T>
{
public:
	inline const LibPoint<T> EndPoint() const
	{
		return this->Origin() + this->Direction().Coordinates();
	}

	bool IsIntersectionLine(const LibLine<T>& lnOther, LibPoint<T>& intersPoint) const override
	{
		double coefL, coefS;
		if (GetIntersParam(lnOther, coefL, coefS))
		{
			if (coefS < 0 || coefS > 1)
			{
				return false;
			}
			return GetIntersection(coefS);
		}
		return false
		
	}

	bool IsIntersectionRay(const LibRay<T>& rayOther, LibPoint<T>& intersPoint) const
	{
		double coefR, coefS;
		if (GetIntersParam(rayOther, coefR, coefS))
		{
			if ((coefS < 0 || coefS > 1) && coefR < 0)
			{
				return false;
			}
			return GetIntersection(coefS);
		}
		return false;
	}

	bool IsIntersectionSegment(const LibRay<T>& sgmntOther, LibPoint<T>& intersPoint) const
	{
		double coef1, coef2;
		if (GetIntersParam(rayOther, coef1, coef2))
		{
			if (coef2 < 0 || coef2 > 1 || coef1 < 0 || coef1 > 1)
			{
				return false;
			}
			return GetIntersection(coef2);
		}
		return false;
	}

	LibPoint<T> ClosestPointOnLine(const LibPoint<T>& point) const override
	{
		LibPoint<T> end = EndPoint();

		LibVector<T> BegPoint = point - this->Origin();
		LibVector<T> BegEnd = this->Direction();

		LibVector<T> projection = BegPoint.DotProduct(BegEnd) / BegEnd.LengthVector();
		if (projection < 0)
		{
			return this->Origin();
		}
		else if (projection > 1) {
			return end;
		}
		else
		{
			return this->Origin() + projection * end;
		}
	}

	bool IsPointOnLine(const LibPoint<T>& point) const override
	{
		LibPoint<T> end = EndPoint();

		LibVector<T> BegPoint = point - this->Origin();
		LibVector<T> BegEnd = this->Direction() - end;

		if (BegPoint.IsParallel(BegEnd))
		{
			if (std::min(this->Origin().X(), end.X()) <= point.X() &&
				std::max(this->Origin().X(), end.X()) >= point.X() &&
				std::min(this->Origin().Y(), end.Y()) <= point.Y() &&
				std::max(this->Origin().Y(), end.Y()) >= point.Y() &&
				std::min(this->Origin().Z(), end.Z()) <= point.Z() &&
				std::max(this->Origin().Z(), end.Z()) >= point.Z())
			{
				return true;
			}
		}
		return false;
	}
};