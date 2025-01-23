#pragma once

#include "libPoint.h"

class LibVector {
public:
	LibVector();
	LibVector(const LibPoint& pt_coordinates);
	~LibVector();

	inline const LibPoint& Coordinates() const
	{
		return m_pt_coord;
	};

	inline double X() const
	{
		return m_pt_coord.X();
	}
	inline double Y() const
	{
		return m_pt_coord.Y();
	}
	inline double Z() const
	{
		return m_pt_coord.Z();
	}

	inline void SetCoordinates(const LibPoint& coordinates)
	{
		m_pt_coord = coordinates;
	};

	LibVector operator*(double scalar);
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

LibVector operator*(double scalar, const LibVector& vec);