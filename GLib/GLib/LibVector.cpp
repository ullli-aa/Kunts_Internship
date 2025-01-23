#pragma once

#include "libVector.h"

LibVector::LibVector() : m_pt_coord(0, 0, 0) {}
LibVector::LibVector(const LibPoint& pt_coordinates)
{
	m_pt_coord = pt_coordinates;
}

LibVector::~LibVector() = default;

LibVector LibVector::operator*(double scalar)
{
	return LibVector(LibPoint(m_pt_coord.X() * scalar, m_pt_coord.Y() * scalar, m_pt_coord.Z() * scalar));
}

LibVector LibVector::operator+(const LibVector & other)
{
	LibPoint other_coord = other.m_pt_coord;
	return LibVector(LibPoint(m_pt_coord.X() + other_coord.X(), m_pt_coord.Y() + other_coord.Y(), m_pt_coord.Z() + other_coord.Z()));
};

LibVector LibVector::operator-(const LibVector & other)
{
	LibPoint other_coord = other.m_pt_coord;
	return LibVector(LibPoint(m_pt_coord.X() - other_coord.X(), m_pt_coord.Y() - other_coord.Y(), m_pt_coord.Z() - other_coord.Z()));
}

bool LibVector::operator==(const LibVector & other)
{
	return m_pt_coord == other.m_pt_coord;
}

bool LibVector::operator!=(const LibVector & other)
{
	return m_pt_coord != other.m_pt_coord;;
}

bool LibVector::IsEqual(const LibVector & other, double eps)
{
	return m_pt_coord.IsEqual(other.m_pt_coord);
}

double LibVector::LengthVector()
{
	return m_pt_coord.DistanceTo(LibPoint(0, 0, 0));
}

void LibVector::Normalize()
{
	double length = LengthVector();
	if (length != 0) {
		m_pt_coord.SetXYZ(m_pt_coord.X() / length, m_pt_coord.Y() / length, m_pt_coord.Z() / length);
	}
}

double LibVector::DotProduct(const LibVector & other)
{
	LibPoint other_coord = other.m_pt_coord;
	return m_pt_coord.X() * other_coord.X() + m_pt_coord.Y() * other_coord.Y() + m_pt_coord.Z() * other_coord.Z();
}

LibVector LibVector::CrossProduct(const LibVector & other)
{
	LibPoint other_coord = other.m_pt_coord;
	return LibVector(LibPoint(
		m_pt_coord.Y() * other_coord.Z() - m_pt_coord.Z() * other_coord.Y(),
		-m_pt_coord.X() * other_coord.Z() + m_pt_coord.Z() * other_coord.X(),
		m_pt_coord.X() * other_coord.Y() - m_pt_coord.Y() * other_coord.X()));
}

LibVector operator*(double scalar, const LibVector & vec)
{
	return LibVector();
}
