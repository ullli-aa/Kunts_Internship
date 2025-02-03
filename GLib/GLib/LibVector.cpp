#pragma once

#include "libVector.h"
#include <cmath>
#include <math.h>

template <typename T>
LibVector<T>::LibVector(const LibPoint<T>& ptCoordinates)
{
	m_PtCoord = ptCoordinates;
}

template<typename T>
bool LibVector<T>::IsParallel(const LibVector& other) const
{
	LibVector crossVec = CrossProduct(other);
	return crossVec.X() <= LibEps::eps &&
		crossVec.Y() <= LibEps::eps &&
		crossVec.Z() <= LibEps::eps;
}

template <typename T>
LibVector<T> LibVector<T>::operator*(T scalar)
{
	return LibVector(LibPoint<T>(m_PtCoord.X() * scalar, m_PtCoord.Y() * scalar, m_PtCoord.Z() * scalar));
}

template<typename T>
LibVector<T> LibVector<T>::operator/(T scalar)
{
	return LibVector(LibPoint<T>(m_PtCoord.X() / scalar, m_PtCoord.Y() / scalar, m_PtCoord.Z() / scalar));
}

template <typename T>
LibVector<T> LibVector<T>::operator+(const LibVector& other)
{
	LibPoint<T> other_coord = other.m_PtCoord;
	return LibVector(LibPoint<T>(m_PtCoord.X() + other_coord.X(), m_PtCoord.Y() + other_coord.Y(), m_PtCoord.Z() + other_coord.Z()));
};

template <typename T>
LibVector<T> LibVector<T>::operator-(const LibVector& other)
{
	LibPoint<T> other_coord = other.m_PtCoord;
	return LibVector(LibPoint<T>(m_PtCoord.X() - other_coord.X(), m_PtCoord.Y() - other_coord.Y(), m_PtCoord.Z() - other_coord.Z()));
}

template <typename T>
bool LibVector<T>::operator==(const LibVector& other)
{
	return IsEqual(other, LibEps::eps);
}

template <typename T>
bool LibVector<T>::operator!=(const LibVector& other)
{
	return !(m_PtCoord == other.m_PtCoord);
}

template <typename T>
bool LibVector<T>::IsEqual(const LibVector& other, double eps) const
{
	return IsParallel(other) &&
		std::fabs(LengthVector() - other.LengthVector()) <= eps;
}

template<typename T>
bool LibVector<T>::IsZero() const
{
	return X <= LibEps::eps && Y <= LibEps::eps && Z <= LibEps::eps;
}

template <typename T>
double LibVector<T>::LengthVector() const
{
	return std::sqrt(m_PtCoord.m_x * m_PtCoord.m_x +
		m_PtCoord.m_y * m_PtCoord.m_y +
		m_PtCoord.m_z * m_PtCoord.m_z);
}

template <typename T>
LibVector<T>& LibVector<T>::NormalizeThis()
{
	double length = LengthVector();
	if (!IsZero())
	{
		m_PtCoord.SetXYZ(m_PtCoord.X() / length, m_PtCoord.Y() / length, m_PtCoord.Z() / length);
	}
	return *this;
}

template<typename T>
LibVector<T> LibVector<T>::GetNormalize() const
{
	double length = LengthVector();
	if (!IsZero())
	{
		return LibVector<T>(LibPoint<T>(m_PtCoord.X() / length, m_PtCoord.Y() / length, m_PtCoord.Z() / length));
	}
	return *this;
}

template <typename T>
double LibVector<T>::DotProduct(const LibVector& other) const
{
	LibPoint<T> other_coord = other.m_PtCoord;
	return m_PtCoord.X() * other_coord.X() + m_PtCoord.Y() * other_coord.Y() + m_PtCoord.Z() * other_coord.Z();
}

template <typename T>
LibVector<T> LibVector<T>::CrossProduct(const LibVector& other) const
{
	LibPoint<T> other_coord = other.m_PtCoord;
	return LibVector(LibPoint<T>(
		m_PtCoord.Y() * other_coord.Z() - m_PtCoord.Z() * other_coord.Y(),
		-m_PtCoord.X() * other_coord.Z() + m_PtCoord.Z() * other_coord.X(),
		m_PtCoord.X() * other_coord.Y() - m_PtCoord.Y() * other_coord.X()));
}

template<typename T>
double LibVector<T>::TripleProduct(const LibVector& first, const LibVector& second) const
{
	auto crossVec = CrossProduct(first);
	return second.DotProduct(crossVec);
}

template<typename T>
T LibVector<T>::AngleBetweenVec(const LibVector& other) const
{
	LibVector<T> vecCross = CrossProduct(other);
	T dotProduct = DotProduct(other);
	return std::acos(dotProduct / (LengthVector() * other.LengthVector()));
}

template<typename T>
T LibVector<T>::AngleTo(const LibVector& vec2, const LibVector& vecDir) const
{
	auto val = AngleBetweenVec(vec2);
	if (!vecDir.IsOpposite(CrossProduct(vec2)))
	{
		return val;
	}

	return 2 * M_PI - val;
}

template<typename T>
bool LibVector<T>::IsOrtogonal(const LibVector& other) const
{
	return DotProduct(other) <= LibEps::eps;
}

template<typename T>
bool LibVector<T>::IsOpposite(const LibVector& other) const
{
	return DotProduct(other) < 0;
}

template<typename T>
LibVector<T> LibVector<T>::Rotate2D(double angle) const
{
	double radians = angle * (M_PI / 180.0);
	double cosAngle = std::cos(radians);
	double sinAngle = std::sin(radians);
	return LibVector(LibPoint<T>(X() * cosAngle - Y() * sinAngle, X() * sinAngle + Y() * cosAngle));
}

template <typename T>
LibVector<T> operator*(double scalar, const LibVector<T>& vec)
{
	return LibVector<T>(LibPoint<T>(vec.m_PtCoord.X() * scalar, vec.m_PtCoord.Y() * scalar, vec.m_PtCoord.Z() * scalar));
}
