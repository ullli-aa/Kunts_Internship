#pragma once

#include "LibPoint.h"
#include "LibVector.h"
#include <variant>

template<typename T>
class LibLine {
public:
	LibLine() = default;
	LibLine(const LibPoint<T>& ptOrigin, const LibVector<T>& ptDirection);

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

	T AngleBetweenLines(const LibLine<T>& lnOther) const;
	
	bool IsParallel(const LibLine<T>& lnOther) const;
	bool IsOrtogonal(const LibLine<T>& lnOther) const;
	bool IsOpposite(const LibLine<T>& lnOther) const;
	bool IsSkew(const LibLine<T>& lnOther) const;

	virtual bool IsIntersectionLine(const LibLine<T>& lnOther, LibPoint<T>& interPoint) const;
	virtual LibPoint<T> ClosestPointOnLine(const LibPoint<T>& point) const;
	virtual bool IsPointOnLine(const LibPoint<T>& point) const;

	double DistBetweenLines(const LibLine<T>& lnOther) const;

protected:
	LibPoint<T> m_ptOrigin;
	LibVector<T> m_ptDirection;
};

