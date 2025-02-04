#pragma once

#include "LibPoint.h"
#include "LibVector.h"

template<typename T>
class LibLine
{
public:
	LibLine() = default;
	LibLine(const LibPoint<T>& ptOrigin, const LibVector<T>& ptDirection)
	{
		m_ptOrigin = ptOrigin;
		m_ptDirection = ptDirection;
	};

	inline const LibPoint<T>& Origin() const
	{
		return m_ptOrigin;
	}
	inline const LibVector<T>& Direction() const
	{
		return m_ptDirection;
	}
	inline void SetOrigin(const LibPoint<T>& ptOrigin)
	{
		m_ptOrigin = ptOrigin;
	}
	inline void SetDirection(const LibVector<T>& ptDirection)
	{
		m_ptDirection = ptDirection;
	}

	T AngleBetweenLines(const LibLine<T>& lnOther) const
	{
		return m_ptDirection.AngleBetweenVec(lnOther.m_ptDirection);
	};

	bool IsParallel(const LibLine<T>& lnOther) const
	{
		return m_ptDirection.IsOpposite(lnOther.m_ptDirection);;
	};
	bool IsOrtogonal(const LibLine<T>& lnOther) const
	{
		return m_ptDirection.IsOrtogonal(lnOther.m_ptDirection);;
	};
	bool IsOpposite(const LibLine<T>& lnOther) const
	{
		return m_ptDirection.IsOpposite(lnOther.m_ptDirection);;
	};
	bool IsSkew(const LibLine<T>& lnOther) const
	{
		bool inters = IsIntersectionLine(lnOther);
		return !IsParallel(lnOther) && !inters;
	};

	bool GetIntersParam(const LibLine<T>& lnOther, T& coef1, T& coef2) const
	{
		if (IsParallel(lnOther))
		{
			return false;
		}

		LibVector<T> vecCross = m_ptDirection.CrossProduct(lnOther.m_ptDirection);

		LibVector<T> vec = m_ptOrigin - lnOther.m_ptOrigin;

		coef1 = vec.CrossProduct(m_ptDirection) / vecCross;
		coef2 = vec.CrossProduct(lnOther.Direction()) / vecCross;
		return true;
	};
	bool GetIntersection(double coef, LibPoint<T>& intersPoint)
	{
		intersPoint = m_ptOrigin + coef * m_ptDirection;
		return true;
	};

	virtual bool IsIntersectionLine(const LibLine<T>& lnOther, LibPoint<T>& intersPoint) const
	{
		T	 coef1, coef2;
		if (GetIntersParam(lnOther, coef1, coef2))
		{
			return GetIntersection(coef2);
		}
		return false;
	};
	virtual LibPoint<T> ClosestPointOnLine(const LibPoint<T>& point) const
	{
		LibVector<T> vec = LibVector<T>(point - m_ptOrigin);
		double coef = vec.DotProduct(m_ptDirection.Normalize());
		LibVector<T> scalDir = coef * m_ptDirection;
		return m_ptOrigin + scalDir.Coordinates();
	};
	virtual bool IsPointOnLine(const LibPoint<T>& point, double eps = LibEps::eps) const
	{
		LibVector<T> vec = LibVector<T>(point - m_ptOrigin);

		if (m_ptDirection.LengthVector() <= LibEps::eps)
		{
			return false;
		}

		return LibEps::IsZero(std::fabs(vec.DotProduct(m_ptDirection) - 1), eps);
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

		//LibVector<T> vec = LibVector<T>(lnOther.Origin() - m_ptOrigin);
		//LibVector<T> vecCross = m_ptDirection.CrossProduct(lnOther.m_ptDirection);

		//return vec.DotProduct(vecCross) / vecCross.LengthVector();
		return 0;
	}

protected:
	LibPoint<T> m_ptOrigin;
	LibVector<T> m_ptDirection;
};

