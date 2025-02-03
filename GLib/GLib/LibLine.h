#pragma once

#include "LibPoint.h"
#include "LibVector.h"
#include <variant>

template<typename T>
class LibLine {
public:
	LibLine() = default;
	LibLine(LibPoint<T> ptOrigin, LibVector<T> ptDirection);

	inline LibPoint<T>& Origin() const
	{
		return m_ptOrigin;
	}
	inline LibVector<T>& Direction() const
	{
		return m_ptDirection;
	}
	inline void SetOrigin(const LibPoint<T>& ptOrigin) const
	{
		m_ptOrigin = ptOrigin;
	}
	inline void SetDirection(const LibVector<T>& ptDirection) const
	{
		m_ptDirection = ptDirection;
	}

	T AngleBetweenLines(const LibLine<T>& lnOther) const;

	bool IsParallel(const LibLine<T>& lnOther) const;
	bool IsOrtogonal(const LibLine<T>& lnOther) const;
	bool IsOpposite(const LibLine<T>& lnOther) const;
	bool IsSkew(const LibLine<T>& lnOther) const;

	virtual std::variant<LibPoint<T>, bool> IsIntersection(const LibLine<T>& lnOther) const;
	virtual LibPoint<T> ClosestPointOnLine(const LibPoint<T>& point) const;
	virtual bool IsPointOnLine(const LibPoint<T>& point);
private:
	LibPoint<T> m_ptOrigin;
	LibVector<T> m_ptDirection;
};

