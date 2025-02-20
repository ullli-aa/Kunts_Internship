#pragma once

#include "LibUtility.h"
#include "LibVector.h"
#include "LibEps.h"
#include <cmath>

template<typename T>
class LibPoint {
public:
	LibPoint() : m_x(0.0), m_y(0.0), m_z(0.0) {};
	LibPoint(T x, T y) : m_x(x), m_y(y), m_z(0.0) {};
	LibPoint(T x, T y, T z) : m_x(x), m_y(y), m_z(z) {};;

	~LibPoint() = default;

	LibVector<T>& AsVector() const {
		return LibVector<T>(m_x, m_y, m_z);
	}

	T SquareDistanceTo(const LibPoint<T>& other) const
	{
		return LibUtility<T>::Square(m_x - other.m_x) +
			LibUtility<T>::Square(m_y - other.m_y) +
			LibUtility<T>::Square(m_z - other.m_z);
	};

	T DistanceTo(const LibPoint<T>& other) const
	{
		return std::sqrt(SquareDistanceTo(other));
	};

	bool operator==(const LibPoint<T>& other) const
	{
		return LibEps::IsZero(SquareDistanceTo(other));
	};

	bool operator!=(const LibPoint<T>& other) const
	{
		return !(*this == other);
	};

	bool IsEqual(const LibPoint<T>& other, double eps = 1e-9) const
	{
		return SquareDistanceTo(other) <= LibUtility<double>::Square(eps);
	};

	inline T X() const
	{
		return m_x;
	};

	inline T Y() const
	{
		return m_y;
	};

	inline T Z() const
	{
		return m_z;
	};

	inline LibPoint<T>& SetX(T x)
	{
		m_x = x;
		return *this;
	};

	inline LibPoint<T>& SetY(T y)
	{
		m_y = y;
		return *this;
	};

	inline LibPoint<T>& SetZ(T z)
	{
		m_z = z;
		return *this;
	};

	inline void SetXYZ(T x = 0.0, T y = 0.0, T z = 0.0)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	};

private:
	T m_x;
	T m_y;
	T m_z;
};

template<typename T>
LibPoint<T> operator+(const LibPoint<T>& point, const LibVector<T>& vec)
{
	return LibPoint<T>(point.X() + vec.X(), point.Y() + vec.Y(), point.Z() + vec.Z());
};

template<typename T>
LibVector<T> operator-(const LibPoint<T>& first, const LibPoint<T>& second)
{
	return LibVector<T>(
		first.X() - second.X(),
		first.Y() - second.Y(),
		first.Z() - second.Z());
};