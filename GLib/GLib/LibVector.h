#pragma once

#include "LibPoint.h"
#include "LibEps.h"
#include <cmath>
#include <math.h>

template<typename T>
class LibVector
{
public:
	LibVector() = default;
	LibVector(const LibPoint<T>& ptCoordinates)
	{
		m_PtCoord = ptCoordinates;
	};
	~LibVector() = default;

	inline const LibPoint<T>& Coordinates() const
	{
		return m_PtCoord;
	};

	inline T X() const
	{
		return m_PtCoord.X();
	}
	inline T Y() const
	{
		return m_PtCoord.Y();
	}
	inline T Z() const
	{
		return m_PtCoord.Z();
	}
	inline const LibVector<T>& SetX(T x)
	{
		m_PtCoord.SetX(x);
		return *this;
	}
	inline const LibVector<T>& SetY(T y)
	{
		m_PtCoord.SetY(y);
		return *this;
	}
	inline const LibVector<T>& SetZ(T z)
	{
		m_PtCoord.SetZ(z);
		return *this;
	}
	inline void SetCoordinates(const LibPoint<T>& coordinates)
	{
		m_PtCoord = coordinates;
	}

	bool IsParallel(const LibVector<T>& other, double eps = LibEps::eps) const
	{
		LibVector crossVec = CrossProduct(other);
		return LibEps::IsZero(crossVec.X(), eps) &&
			LibEps::IsZero(crossVec.Y(), eps) &&
			LibEps::IsZero(crossVec.Z(), eps);
	};
	
	LibVector<T> operator*(T scalar)
	{
		return LibVector(LibPoint<T>(m_PtCoord.X() * scalar, m_PtCoord.Y() * scalar, m_PtCoord.Z() * scalar));
	};
	LibVector<T> operator/(T scalar)
	{
		return LibVector(LibPoint<T>(m_PtCoord.X() / scalar, m_PtCoord.Y() / scalar, m_PtCoord.Z() / scalar));
	};
	LibVector<T> operator+(const LibVector<T>& other)
	{
		return m_PtCoord + other;
	};
	
	LibVector<T> operator-(const LibVector<T>& other)
	{
		return m_PtCoord - other.Coordinates();
	};
	bool operator==(const LibVector<T>& other)
	{
		return IsEqual(other, LibEps::eps);
	};
	bool operator!=(const LibVector<T>& other)
	{
		return !(m_PtCoord == other.m_PtCoord);
	};
	bool IsEqual(const LibVector<T>& other, double eps = LibEps::eps) const
	{
		return IsParallel(other) && LibEps::IsZero(std::fabs(LengthVector() - other.LengthVector()), eps);
	};

	bool IsZero() const
	{
		return LibEps::IsZero(X) && LibEps::IsZero(Y) && LibEps::IsZero(Z);
	};

	double LengthVector() const
	{
		return std::sqrt(m_PtCoord.m_x * m_PtCoord.m_x +
						 m_PtCoord.m_y * m_PtCoord.m_y +
						 m_PtCoord.m_z * m_PtCoord.m_z);
	};
	LibVector<T>& NormalizeThis()
	{
		*this = GetNormalize();
		return *this;
	};
	LibVector<T> GetNormalize() const
	{
		double length = LengthVector();
		if (!IsZero())
		{
			return LibVector<T>(LibPoint<T>(m_PtCoord.X() / length, m_PtCoord.Y() / length, m_PtCoord.Z() / length));
		}
		return *this;
	};

	double DotProduct(const LibVector<T>& other) const
	{
		return m_PtCoord.X() * other.X() + m_PtCoord.Y() * other.Y() + m_PtCoord.Z() * other.Z();
	};
	LibVector<T> CrossProduct(const LibVector<T>& other) const
	{
		return LibVector(LibPoint<T>(
			m_PtCoord.Y() * other.Z() - m_PtCoord.Z() * other.Y(),
			-m_PtCoord.X() * other.Z() + m_PtCoord.Z() * other.X(),
			m_PtCoord.X() * other.Y() - m_PtCoord.Y() * other.X()));
	};
	double TripleProduct(const LibVector<T>& first, const LibVector<T>& second) const
	{
		auto crossVec = CrossProduct(first);
		return second.DotProduct(crossVec);
	};

	T AngleBetweenVec(const LibVector<T>& other) const
	{
		T dotProduct = DotProduct(other);
		return std::acos(dotProduct / (LengthVector() * other.LengthVector()));
	};
	T AngleTo(const LibVector<T>& vec2, const LibVector<T>& vecDir) const
	{
		auto val = AngleBetweenVec(vec2);
		if (!vecDir.IsOpposite(CrossProduct(vec2)))
		{
			return val;
		}

		return 2 * M_PI - val;
	};

	bool IsOrtogonal(const LibVector<T>& other, double eps = LibEps::eps) const
	{
		return LibEps::IsZero(std::fabs(DotProduct(other)), eps);
	};
	bool IsOpposite(const LibVector<T>& other) const
	{
		return DotProduct(other) < 0 && IsParallel(other);
	};

	LibVector<T> Rotate2D(double angle) const
	{
		double radians = angle * (M_PI / 180.0);
		double cosAngle = std::cos(radians);
		double sinAngle = std::sin(radians);
		return LibVector(LibPoint<T>(X() * cosAngle - Y() * sinAngle, X() * sinAngle + Y() * cosAngle));
	};

private:
	LibPoint<T> m_PtCoord;
};

template<typename T>
LibVector<T> operator*(double scalar, const LibVector<T>& vec)
{
	return vec * scalar;
};

template<typename T>
LibPoint<T> operator+(const LibPoint<T>& point, const LibVector<T>& vec)
{
	return LibPoint<T>(point.m_x + vec.X(), point.m_y + vec.Y(), point.m_z + vec.Z());
}

template<typename T>
LibVector<T> operator-(const LibPoint<T>& first, const LibPoint<T>& second)
{
	return LibVector<T>(first.m_x - second.X, first.m_y - second.Y, first.m_z - second.Z);
}