#pragma once

#include "LibCoordinates.h"
#include "LibEps.h"

template<typename T>
class LibVector : public LibCoordinates<T>
{
public:
	LibVector() :LibCoordinates<T>() {};
	LibVector(T x, T y) : LibCoordinates<T>(x, y) {};
	LibVector(T x, T y, T z) : LibCoordinates<T>(x, y, z) {};
	~LibVector() = default;

	bool IsParallel(const LibVector<T>& other, double eps = LibEps::eps) const
	{
		LibVector crossVec = CrossProduct(other);
		return LibEps::IsZero(crossVec.X(), eps) &&
			LibEps::IsZero(crossVec.Y(), eps) &&
			LibEps::IsZero(crossVec.Z(), eps);
	};

	LibVector<T> operator*(T scalar) const
	{
		return LibVector<T>(this->X() * scalar, this->Y() * scalar, this->Z() * scalar);
	}

	LibVector<T> operator/(T scalar) const
	{
		return LibVector(this->X() / scalar, this->Y() / scalar, this->Z() / scalar);
	};

	LibVector<T> operator+(const LibVector<T>& other) const
	{
		return LibVector<T>(this->m_x + other.X(), this->m_y + other.Y(), this->m_z + other.Z());
	};

	void operator+=(const LibVector<T>& other)
	{
		*this = *this + other;
	};

	LibVector<T> operator-(const LibVector<T>& other) const
	{
		return LibVector<T>(this->m_x - other.X(), this->m_y - other.Y(), this->m_z - other.Z());
	};

	bool operator==(const LibVector<T>& other) const
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
		return LibEps::IsZero(this->X(), eps) &&
			LibEps::IsZero(this->Y(), eps) &&
			LibEps::IsZero(this->Z(), eps);
	};

	double LengthVector() const
	{
		return std::sqrt(LengthVectorPow2());
	};

	double LengthVectorPow2() const
	{
		return this->X() * this->X() +
			this->Y() * this->Y() +
			this->Z() * this->Z();
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
			return LibVector<T>(this->X() / length, this->Y() / length, this->Z() / length);
		}
		return *this;
	};

	double DotProduct(const LibVector<T>& other) const
	{
		return this->X() * other.X() + this->Y() * other.Y() + this->Z() * other.Z();
	};

	LibVector<T> CrossProduct(const LibVector<T>& other) const
	{
		return LibVector(
			this->Y() * other.Z() - this->Z() * other.Y(),
			-this->X() * other.Z() + this->Z() * other.X(),
			this->X() * other.Y() - this->Y() * other.X());
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
		if (LibEps::IsZero(this->X())) {
			if (!LibEps::IsZero(this->Y())) {
				return LibVector<T>(0, this->Z(), -this->Y());
			}
			return LibVector<T>(0, this->Z(), 0);

		}
		return LibVector<T>(this->Y(), -this->X(), 0);
	}

	bool IsOpposite(const LibVector<T>& other, double eps = LibEps::eps) const
	{
		return DotProduct(other) < 0 && IsParallel(other, eps);
	};
};

template<typename T, typename S>
LibVector<T> operator*(S scalar, const LibVector<T>& vec)
{
	return vec * scalar;
};