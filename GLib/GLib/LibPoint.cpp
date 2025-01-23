#pragma once

#include <cmath>
#include "libPoint.h"
#include "libUtility.h"

LibPoint::LibPoint() = default;

LibPoint::LibPoint(double x, double y) : m_x(x), m_y(y) {}

LibPoint::LibPoint(double x, double y, double z) : m_x(x), m_y(y), m_z(z) {};

LibPoint::~LibPoint() = default;

double LibPoint::SquareDistanceTo(const LibPoint & other) const
{
	return LibUtility::Square(m_x - other.m_x) +
		LibUtility::Square(m_y - other.m_y) +
		LibUtility::Square(m_z - other.m_z);
};

double LibPoint::DistanceTo(const LibPoint & other) const
{
	return std::sqrt(SquareDistanceTo(other));
};


bool LibPoint::operator==(const LibPoint & other) const
{
	return m_x == other.m_x && m_y == other.m_y && m_z == other.m_z;
};

bool LibPoint::operator!=(const LibPoint & other) const
{
	return !(*this == other);
};

bool LibPoint::IsEqual(const LibPoint & other, double eps) const
{
	return SquareDistanceTo(other) <= LibUtility::Square(eps);
};


