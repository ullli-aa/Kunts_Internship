#pragma once

#include "LibEps.h"
#include <cmath>
#include <numbers>

double M_PI = std::numbers::pi;

template<typename T>
class LibVector
{
public:
	LibVector() = default;
	LibVector(T x, T y) : m_x(x), m_y(y) {};
	LibVector(T x, T y, T z) : m_x(x), m_y(y), m_z(z) {};
	~LibVector() = default;

	inline T X() const
	{
		return m_x;
	}
	inline T Y() const
	{
		return m_y;
	}
	inline T Z() const
	{
		return m_z;
	}
	inline LibVector<T>& SetX(T x)
	{
		m_x = x;
		return *this;
	}
	inline LibVector<T>& SetY(T y)
	{
		m_y = y;
		return *this;
	}
	inline LibVector<T>& SetZ(T z)
	{
		m_z = z;
		return *this;
	}

	bool IsParallel(const LibVector<T>& other, double eps = LibEps::eps) const
	{
		LibVector crossVec = CrossProduct(other);
		return LibEps::IsZero(crossVec.X(), eps) &&
			LibEps::IsZero(crossVec.Y(), eps) &&
			LibEps::IsZero(crossVec.Z(), eps);
	};

	LibVector<T> operator*(T scalar) const
	{
		return LibVector<T>(X() * scalar, Y() * scalar, Z() * scalar);
	}

	LibVector<T> operator/(T scalar) const
	{
		return LibVector(X() / scalar, Y() / scalar, Z() / scalar);
	};

	LibVector<T> operator+(const LibVector<T>& other) const
	{
		return LibVector<T>(m_x + other.X(), m_y + other.Y(), m_z + other.Z());
	};

	LibVector<T> operator-(const LibVector<T>& other) const
	{
		return LibVector<T>(m_x - other.X(), m_y - other.Y(), m_z - other.Z());
	};

	bool operator==(const LibVector<T>& other)
	{
		return IsEqual(other, LibEps::eps);
	};

	bool operator!=(const LibVector<T>& other)
	{
		return !(*this == other);
	};

	bool IsEqual(const LibVector<T>& other, double eps = LibEps::eps) const
	{
		return IsParallel(other, eps) && LibEps::IsZero(std::fabs(LengthVector() - other.LengthVector()), eps);
	};

	bool IsZero(double eps = LibEps::eps) const
	{
		return LibEps::IsZero(X(), eps) && LibEps::IsZero(Y(), eps) && LibEps::IsZero(Z(), eps);
	};

	double LengthVector() const
	{
		return std::sqrt(LengthVectorPow2());
	};

	double LengthVectorPow2() const
	{
		return X() * X() +
			Y() * Y() +
			Z() * Z();
	};

	LibVector<T> ProjectionOnto(const LibVector<T>& other) const {
		T dotProduct = DotProduct(other);
		T otherLengthSquared = other.LengthVectorPow2();
		return (dotProduct / otherLengthSquared) * other;
	}

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
			return LibVector<T>(X() / length, Y() / length, Z() / length);
		}
		return *this;
	};

	double DotProduct(const LibVector<T>& other) const
	{
		return X() * other.X() + Y() * other.Y() + Z() * other.Z();
	};

	LibVector<T> CrossProduct(const LibVector<T>& other) const
	{
		return LibVector(
			Y() * other.Z() - Z() * other.Y(),
			-X() * other.Z() + Z() * other.X(),
			X() * other.Y() - Y() * other.X());
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

	T AngleTo(const LibVector<T>& vec2, const LibVector<T>& vecDir, double eps = LibEps::eps) const
	{
		auto val = AngleBetweenVec(vec2);
		if (!vecDir.IsOpposite(CrossProduct(vec2)), eps)
		{
			return val;
		}

		return 2 * M_PI - val;
	};

	bool IsOrtogonal(const LibVector<T>& other, double eps = LibEps::eps) const
	{
		return LibEps::IsZero(std::fabs(DotProduct(other)), eps);
	};

	LibVector<T> GetOrtogonalVec(const double eps = LibEps::eps) const {
		if (LibEps::IsZero(X())) {
			return LibVector<T>(0, Z(), -Y());
		}
		return LibVector<T>(Y(), -X(), 0);
	}

	bool IsOpposite(const LibVector<T>& other, double eps = LibEps::eps) const
	{
		return DotProduct(other) < 0 && IsParallel(other, eps);
	};

	LibVector<T> Rotate2D(double angle) const
	{
		double radians = angle * (M_PI / 180.0);
		double cosAngle = std::cos(radians);
		double sinAngle = std::sin(radians);
		return LibVector(LibPoint<T>(X() * cosAngle - Y() * sinAngle, X() * sinAngle + Y() * cosAngle));
	};

private:
	T m_x;
	T m_y;
	T m_z;
};

template<typename T, typename S>
LibVector<T> operator*(S scalar, const LibVector<T>& vec)
{
	return vec * scalar;
};