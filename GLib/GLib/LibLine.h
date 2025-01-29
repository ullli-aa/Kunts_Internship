#pragma once

#include "LibPoint.h"
#include "LibVector.h"

template<typename T>
class LibLine {
public:
	LibLine() = default;
	LibLine(LibPoint<T> ptOrigin, LibVector<T> ptDirection);

	inline LibPoint& Origin() const
	{
		return m_ptOrigin;
	}
	inline LibVector& Direction() const
	{
		return m_ptDirection;
	}
	inline void SetOrigin(const LibPoint& ptOrigin) const
	{
		m_ptOrigin = ptOrigin;
	}
	inline void SetDirection(const LibVector& ptDirection) const
	{
		m_ptDirection = ptDirection;
	}

	T AngleBetweenLines(const LibLine& lnOther) const;

	bool IsParallel(const LibLine& lnOther) const;
	bool IsOrtogonal(const LibLine& lnOther) const;
	bool IsOpposite(const LibLine& lnOther) const;

	virtual bool IsIntersection(const LibLine& lnOther) const;
private:
	LibPoint<T> m_ptOrigin;
	LibVector<T> m_ptDirection;
};

