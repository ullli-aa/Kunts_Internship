#pragma once

#include "LibPoint.h"
#include "LibEps.h"

template<typename T>
class LibVector {
public:
	LibVector() = default;
	LibVector(const LibPoint<T>& ptCoordinates);
	~LibVector() = default;

	inline const LibPoint<T>& Coordinates() const
	{
		return m_PtCoord;
	};

	inline T X() const
	{
		return m_PtCoord.X();
	}
	inline T Y() const
	{
		return m_PtCoord.Y();
	}
	inline T Z() const
	{
		return m_PtCoord.Z();
	}
	inline const LibVector& SetX(T x)
	{
		m_PtCoord.SetX(x);
		return *this;
	}
	inline const LibVector& SetY(T y)
	{
		m_PtCoord.SetY(y);
		return *this;
	}
	inline const LibVector& SetZ(T z)
	{
		m_PtCoord.SetZ(z);
		return *this;
	}
	inline void SetCoordinates(const LibPoint<T>& coordinates)
	{
		m_PtCoord = coordinates;
	}

	bool IsParallel(const LibVector& other, double eps = LibEps::eps) const;
	LibVector<T> operator*(T scalar);
	LibVector<T> operator/(T scalar);
	LibVector<T> operator+(const LibVector& other);
	LibVector<T> operator-(const LibVector& other);
	bool operator==(const LibVector& other);
	bool operator!=(const LibVector& other);
	bool IsEqual(const LibVector& other, double eps = LibEps::eps) const;
	bool IsZero() const;

	double LengthVector() const;
	LibVector<T>& NormalizeThis();
	LibVector<T> GetNormalize() const;

	double DotProduct(const LibVector& other) const;
	LibVector<T> CrossProduct(const LibVector& other) const;
	double TripleProduct(const LibVector& first, const LibVector& second) const;

	T AngleBetweenVec(const LibVector& other) const;
	T AngleTo(const LibVector& vec2, const LibVector& vecDir) const;

	bool IsOrtogonal(const LibVector& other, double eps = LibEps::eps) const;
	bool IsOpposite(const LibVector& other) const;

	LibVector<T> Rotate2D(double angle) const;

private:
	LibPoint<T> m_PtCoord;
};

template<typename T>
LibVector<T> operator*(double scalar, const LibVector<T>& vec);