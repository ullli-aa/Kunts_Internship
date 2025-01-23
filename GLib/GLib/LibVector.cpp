#pragma once

#include "libVector.h"

template <typename T>
LibVector<T>::LibVector(const LibPoint<T>& ptCoordinates)
{
	m_PtCoord = ptCoordinates;
}

template<typename T>
bool LibVector<T>::IsCollinear(const LibVector& other) const
{
	LibVector crossVec = CrossProduct(other);
	return crossVec.X() <= 1e-9 &&
		crossVec.Y() <= 1e-9 &&
		crossVec.Z() <= 1e-9;
}

template <typename T>
LibVector<T> LibVector<T>::operator*(T scalar)
{
	return LibVector(LibPoint(m_PtCoord.X() * scalar, m_PtCoord.Y() * scalar, m_PtCoord.Z() * scalar));
}

template<typename T>
LibVector<T> LibVector<T>::operator/(T scalar)
{
	return LibVector(LibPoint(m_PtCoord.X() / scalar, m_PtCoord.Y() / scalar, m_PtCoord.Z() / scalar));
}

template <typename T>
LibVector<T> LibVector<T>::operator+(const LibVector& other)
{
	LibPoint other_coord = other.m_PtCoord;
	return LibVector(LibPoint(m_PtCoord.X() + other_coord.X(), m_PtCoord.Y() + other_coord.Y(), m_PtCoord.Z() + other_coord.Z()));
};

template <typename T>
LibVector<T> LibVector<T>::operator-(const LibVector& other)
{
	LibPoint other_coord = other.m_PtCoord;
	return LibVector(LibPoint(m_PtCoord.X() - other_coord.X(), m_PtCoord.Y() - other_coord.Y(), m_PtCoord.Z() - other_coord.Z()));
}

template <typename T>
bool LibVector<T>::operator==(const LibVector& other)
{
	return IsEqual(other, 1e-9);
}

template <typename T>
bool LibVector<T>::operator!=(const LibVector& other)
{
	return m_PtCoord != other.m_PtCoord;;
}

template <typename T>
bool LibVector<T>::IsEqual(const LibVector& other, double eps) const
{
	return IsCollinear(other) &&
		   std::fabs(LengthVector() - other.LengthVector()) <= 1e-9;
}

template <typename T>
double LibVector<T>::LengthVector() const
{
	return m_PtCoord.DistanceTo(LibPoint(0, 0, 0));
}

template <typename T>
void LibVector<T>::Normalize()
{
	double length = LengthVector();
	if (length != 0) {
		m_PtCoord.SetXYZ(m_PtCoord.X() / length, m_PtCoord.Y() / length, m_PtCoord.Z() / length);
	}
}

template <typename T>
double LibVector<T>::DotProduct(const LibVector& other) const
{
	LibPoint other_coord = other.m_PtCoord;
	return m_PtCoord.X() * other_coord.X() + m_PtCoord.Y() * other_coord.Y() + m_PtCoord.Z() * other_coord.Z();
}

template <typename T>
LibVector<T> LibVector<T>::CrossProduct(const LibVector& other) const
{
	LibPoint other_coord = other.m_PtCoord;
	return LibVector(LibPoint(
		m_PtCoord.Y() * other_coord.Z() - m_PtCoord.Z() * other_coord.Y(),
		-m_PtCoord.X() * other_coord.Z() + m_PtCoord.Z() * other_coord.X(),
		m_PtCoord.X() * other_coord.Y() - m_PtCoord.Y() * other_coord.X()));
}

template<typename T>
T LibVector<T>::AngleBetweenVec(const LibVector& other) const
{
	T dotProduct = DotProduct(other);
	return dotProduct / (LengthVector * other.LengthVector);
}

template<typename T>
T LibVector<T>::DistBetweenVec(const LibVector& other) const
{
	return m_PtCoord.DistanceTo(other.m_PtCoord);
}

template<typename T>
LibVector<T> LibVector<T>::Rotate2D(double angle) const
{
	double radians = angle * (M_PI / 180.0);
	double cosAngle = std::cos(radians);
	double sinAngle = std::sin(radians);
	return LibVector(LibPoint(X() * cosAngle - Y() * sinAngle, X() * sinAngle + Y() * cosAngle));
}

template <typename T>
LibVector<T> operator*(double scalar, const LibVector<T>& vec)
{
	return LibVector();
}
