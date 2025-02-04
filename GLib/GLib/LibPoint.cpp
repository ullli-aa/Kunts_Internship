#pragma once

#include <cmath>
#include "LibPoint.h"
#include "LibUtility.h"

template<typename T>
LibPoint<T>::LibPoint() : m_x(0.0), m_y(0.0), m_z(0.0) {}

template<typename T>
LibPoint<T>::LibPoint(T x, T y) : m_x(x), m_y(y) {}

template<typename T>
LibPoint<T>::LibPoint(T x, T y, T z) : m_x(x), m_y(y), m_z(z) {};

template<typename T>
T LibPoint<T>::SquareDistanceTo(const LibPoint& other) const
{
	return LibUtility::Square(m_x - other.m_x) +
		LibUtility::Square(m_y - other.m_y) +
		LibUtility::Square(m_z - other.m_z);
};

template<typename T>
T LibPoint<T>::DistanceTo(const LibPoint& other) const
{
	return std::sqrt(SquareDistanceTo(other));
};

template<typename T>
bool LibPoint<T>::operator==(const LibPoint& other) const
{
	return LibEps::IsZero(SquareDistanceTo(other));
};

template<typename T>
bool LibPoint<T>::operator!=(const LibPoint& other) const
{
	return !(*this == other);
}

template<typename T>
bool LibPoint<T>::IsEqual(const LibPoint& other, double eps) const
{
	return SquareDistanceTo(other) <= LibUtility::Square(eps);
};


