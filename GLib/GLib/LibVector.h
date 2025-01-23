#pragma once

#include "libPoint.h"

template<typename T>
class LibVector {
public:
	LibVector() = default;
	LibVector(const LibPoint& pt_coordinates);
	~LibVector() = default;

	inline const LibPoint& Coordinates() const
	{
		return m_pt_coord;
	};

	inline T X() const
	{
		return m_pt_coord.X();
	}
	inline T Y() const
	{
		return m_pt_coord.Y();
	}
	inline T Z() const
	{
		return m_pt_coord.Z();
	}
	inline LibVector& SetX(T x)
	{
		m_pt_coord.SetX(x);
		return *this;
	}
	inline LibVector& SetY(T y)
	{
		m_pt_coord.SetY(y);
		return *this;
	}
	inline LibVector& SetZ(T z)
	{
		m_pt_coord.SetZ(z);
		return *this;
	}
	inline void SetCoordinates(const LibPoint& coordinates)
	{
		m_pt_coord = coordinates;
	};

	LibVector operator*(T scalar);
	LibVector operator+(const LibVector& other);
	LibVector operator-(const LibVector& other);
	bool operator==(const LibVector& other);
	bool operator!=(const LibVector& other);
	bool IsEqual(const LibVector& other, double eps = 1e-9);

	double LengthVector();
	void Normalize();

	double DotProduct(const LibVector& other);
	LibVector CrossProduct(const LibVector& other);

private:
	LibPoint m_pt_coord;
};

template<typename T>
LibVector<T> operator*(double scalar, const LibVector<T>& vec);