#pragma once

#include "LibUtility.h"
#include "LibVector.h"
#include "LibCoordinates.h"
#include "LibEps.h"

template<typename T>
class LibPoint : public LibCoordinates<T> {
public:
	LibPoint() : LibCoordinates<T>() {};
	LibPoint(T x, T y) : LibCoordinates<T>(x, y) {};
	LibPoint(T x, T y, T z) : LibCoordinates<T>(x, y, z) {};

	~LibPoint() = default;

	LibVector<T> AsVector() const {
		return LibVector<T>(this->m_x, this->m_y, this->m_z);
	}

	T SquareDistanceTo(const LibPoint<T>& other) const
	{
		return LibUtility::Square(this->m_x - other.X()) +
			LibUtility::Square(this->m_y - other.Y()) +
			LibUtility::Square(this->m_z - other.Z());
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
		return SquareDistanceTo(other) <= LibUtility::Square(eps);
	};

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