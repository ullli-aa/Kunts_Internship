#pragma once

#include "LibPoint.h"
#include "LibVector.h"

template<typename T>
class LibLine
{
public:
	LibLine() = default;
	LibLine(const LibPoint<T>& ptOrigin, const LibVector<T>& vecDirection)
	{
		m_ptOrigin = ptOrigin;
		m_vecDirection = vecDirection;
	};

	inline const LibPoint<T>& Origin() const
	{
		return m_ptOrigin;
	}

	inline const LibVector<T>& Direction() const
	{
		return m_vecDirection;
	}

	inline void SetOrigin(const LibPoint<T>& ptOrigin)
	{
		m_ptOrigin = ptOrigin;
	}

	inline void SetDirection(const LibVector<T>& vecDirection)
	{
		m_vecDirection = vecDirection;
	}

	T AngleBetweenLines(const LibLine<T>& lnOther) const
	{
		return m_vecDirection.AngleBetweenVec(lnOther.m_vecDirection);
	};

	bool IsParallel(const LibLine<T>& lnOther) const
	{
		return m_vecDirection.IsParallel(lnOther.m_vecDirection);
	};

	bool IsOrtogonal(const LibLine<T>& lnOther) const
	{
		return m_vecDirection.IsOrtogonal(lnOther.m_vecDirection);
	};

	bool IsOpposite(const LibLine<T>& lnOther) const
	{
		return m_vecDirection.IsOpposite(lnOther.m_vecDirection);
	};

	bool IsSkew(const LibLine<T>& lnOther) const
	{
		LibPoint<T> pt;
		bool inters = IsIntersectionLine(lnOther, pt);
		return !IsParallel(lnOther) && !inters;
	};

	virtual bool IsIntersectionLine(const LibLine<T>& lnOther, LibPoint<T>& intersPoint) const
	{
		T coef1, coef2;
		if (GetIntersParam(lnOther, coef1, coef2))
		{
			return GetIntersection(coef2, intersPoint);
		}
		return false;
	};

	virtual LibPoint<T> ClosestPointOnLine(const LibPoint<T>& point) const
	{
		double coef = GetCoefClosestPoint(point);
		return GetClosestPoint(coef);
	};

	virtual bool IsPointOnLine(const LibPoint<T>& point, double eps = LibEps::eps) const
	{
		LibVector<T> vec = LibVector<T>(point - m_ptOrigin);

		if (m_vecDirection.LengthVector() <= LibEps::eps)
		{
			return false;
		}

		return LibEps::IsZero(std::fabs(vec.DotProduct(m_vecDirection) - 1), eps);
	};

	double DistBetweenLines(const LibLine<T>& lnOther) const
	{
		if (IsIntersectionLine(lnOther))
		{
			return 0;
		}
		if (IsParallel(lnOther))
		{
			LibPoint<T> closestPoint = ClosestPointOnLine(lnOther.Origin());
			LibVector<T> dist = LibVector<T>(m_ptOrigin - closestPoint);
			return dist.LengthVector();
		}

		//ищем вектор перпендикулярный плоскости, которая проходит через lnOther и параллельно this
		LibVector<T> vecCross = m_vecDirection.CrossProduct(lnOther.m_ptDirection);
		vecCross.NormalizeThis();

		//свободный член уравнения плоскости
		double coef = vecCross.DotProduct(lnOther.Origin());

		return vecCross.DotProduct(this->Origin()) + coef;
	}

protected:
	bool GetIntersParam(const LibLine<T>& lnOther, T& coef1, T& coef2) const
	{
		if (IsParallel(lnOther))
		{
			return false;
		}

		LibVector<T> vecCross = m_vecDirection.CrossProduct(lnOther.m_vecDirection);

		LibVector<T> vec = m_ptOrigin - lnOther.m_ptOrigin;

		coef1 = vec.CrossProduct(Direction()).LengthVector() / vecCross.LengthVector();
		coef2 = vec.CrossProduct(lnOther.Direction()).LengthVector() / vecCross.LengthVector();
		return true;
	};

	bool GetIntersection(T coef, LibPoint<T>& intersPoint) const
	{
		intersPoint = m_ptOrigin + coef * m_vecDirection;
		return true;
	};

	double GetCoefClosestPoint(const LibPoint<T>& point) const
	{
		LibVector<T> vec = LibVector<T>(point - m_ptOrigin);
		return vec.DotProduct(m_vecDirection.GetNormalize());
	}

	LibPoint<T> GetClosestPoint(double coef) const
	{
		return m_ptOrigin + coef * m_vecDirection;
	}

protected:
	LibPoint<T> m_ptOrigin;
	LibVector<T> m_vecDirection;
};

