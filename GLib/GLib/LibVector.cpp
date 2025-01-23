#pragma once

#include "libVector.h"

template <typename T>
LibVector<T>::LibVector(const LibPoint<T>& pt_coordinates)
{
	m_pt_coord = pt_coordinates;
}

template <typename T>
LibVector<T> LibVector<T>::operator*(T scalar)
{
	return LibVector(LibPoint(m_pt_coord.X() * scalar, m_pt_coord.Y() * scalar, m_pt_coord.Z() * scalar));
}

template <typename T>
LibVector<T> LibVector<T>::operator+(const LibVector & other)
{
	LibPoint other_coord = other.m_pt_coord;
	return LibVector(LibPoint(m_pt_coord.X() + other_coord.X(), m_pt_coord.Y() + other_coord.Y(), m_pt_coord.Z() + other_coord.Z()));
};

template <typename T>
LibVector<T> LibVector<T>::operator-(const LibVector & other)
{
	LibPoint other_coord = other.m_pt_coord;
	return LibVector(LibPoint(m_pt_coord.X() - other_coord.X(), m_pt_coord.Y() - other_coord.Y(), m_pt_coord.Z() - other_coord.Z()));
}

template <typename T>
bool LibVector<T>::operator==(const LibVector & other)
{
	return m_pt_coord == other.m_pt_coord;
}

template <typename T>
bool LibVector<T>::operator!=(const LibVector & other)
{
	return m_pt_coord != other.m_pt_coord;;
}

template <typename T>
bool LibVector<T>::IsEqual(const LibVector & other, double eps)
{
	return m_pt_coord.IsEqual(other.m_pt_coord);
}

template <typename T>
double LibVector<T>::LengthVector()
{
	return m_pt_coord.DistanceTo(LibPoint(0, 0, 0));
}

template <typename T>
void LibVector<T>::Normalize()
{
	double length = LengthVector();
	if (length != 0) {
		m_pt_coord.SetXYZ(m_pt_coord.X() / length, m_pt_coord.Y() / length, m_pt_coord.Z() / length);
	}
}

template <typename T>
double LibVector<T>::DotProduct(const LibVector & other)
{
	LibPoint other_coord = other.m_pt_coord;
	return m_pt_coord.X() * other_coord.X() + m_pt_coord.Y() * other_coord.Y() + m_pt_coord.Z() * other_coord.Z();
}

template <typename T>
LibVector<T> LibVector<T>::CrossProduct(const LibVector & other)
{
	LibPoint other_coord = other.m_pt_coord;
	return LibVector(LibPoint(
		m_pt_coord.Y() * other_coord.Z() - m_pt_coord.Z() * other_coord.Y(),
		-m_pt_coord.X() * other_coord.Z() + m_pt_coord.Z() * other_coord.X(),
		m_pt_coord.X() * other_coord.Y() - m_pt_coord.Y() * other_coord.X()));
}

template <typename T>
LibVector<T> operator*(double scalar, const LibVector<T> & vec)
{
	return LibVector();
}
