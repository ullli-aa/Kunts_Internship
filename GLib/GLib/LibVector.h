#pragma once

#include "libPoint.h"

template<typename T>
class LibVector {
public:
	LibVector() = default;
	LibVector(const LibPoint& ptCoordinates);
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
	inline LibVector& SetX(T x)
	{
		m_PtCoord.SetX(x);
		return *this;
	}
	inline LibVector& SetY(T y)
	{
		m_PtCoord.SetY(y);
		return *this;
	}
	inline LibVector& SetZ(T z)
	{
		m_PtCoord.SetZ(z);
		return *this;
	}
	inline void SetCoordinates(const LibPoint& coordinates)
	{
		m_PtCoord = coordinates;
	}

	bool IsCollinear(const LibVector& other) const;
	LibVector<T> operator*(T scalar);
	LibVector<T> operator/(T scalar);
	LibVector<T> operator+(const LibVector& other);
	LibVector<T> operator-(const LibVector& other);
	bool operator==(const LibVector& other);
	bool operator!=(const LibVector& other);
	bool IsEqual(const LibVector& other, double eps = 1e-9) const;

	double LengthVector() const;
	void Normalize();

	double DotProduct(const LibVector& other) const;
	LibVector<T> CrossProduct(const LibVector& other) const;

	T AngleBetweenVec(const LibVector& other) const;
	T DistBetweenVec(const LibVector& other) const;

	LibVector<T> Rotate2D(double angle) const;

private:
	LibPoint<T> m_PtCoord;
};

template<typename T>
LibVector<T> operator*(double scalar, const LibVector<T>& vec);