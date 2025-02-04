#pragma once

#include "LibVector.h"
#include <cmath>
#include <math.h>

template <typename T>
LibVector<T>::LibVector(const LibPoint<T>& ptCoordinates)
{
	m_PtCoord = ptCoordinates;
}

template<typename T>
bool LibVector<T>::IsParallel(const LibVector& other, double eps) const
{
	LibVector crossVec = CrossProduct(other);
	return LibEps::IsZero(crossVec.X(), eps) &&
		LibEps::IsZero(crossVec.Y(), eps) &&
		LibEps::IsZero(crossVec.Z(), eps);
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
	return m_PtCoord + other;
};

template <typename T>
LibVector<T> LibVector<T>::operator-(const LibVector& other)
{
	return m_PtCoord - other.Coordinates();
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
	return IsParallel(other) && LibEps::IsZero(std::fabs(LengthVector() - other.LengthVector()), eps);
}

template<typename T>
bool LibVector<T>::IsZero() const
{
	return LibEps::IsZero(X) && LibEps::IsZero(Y) && LibEps::IsZero(Z);
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
	*this = GetNormalize();
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
	return m_PtCoord.X() * other.X() + m_PtCoord.Y() * other.Y() + m_PtCoord.Z() * other.Z();
}

template <typename T>
LibVector<T> LibVector<T>::CrossProduct(const LibVector& other) const
{
	return LibVector(LibPoint<T>(
		m_PtCoord.Y() * other.Z() - m_PtCoord.Z() * other.Y(),
		-m_PtCoord.X() * other.Z() + m_PtCoord.Z() * other.X(),
		m_PtCoord.X() * other.Y() - m_PtCoord.Y() * other.X()));
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
bool LibVector<T>::IsOrtogonal(const LibVector& other, double eps) const
{
	return LibEps::IsZero(std::fabs(DotProduct(other)), eps);
}

template<typename T>
bool LibVector<T>::IsOpposite(const LibVector& other) const
{
	return DotProduct(other) < 0 && IsParallel(other);
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
	return vec * scalar;
}
