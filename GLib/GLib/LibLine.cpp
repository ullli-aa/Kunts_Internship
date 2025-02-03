#include "LibLine.h"
#include <cmath>

template<typename T>
LibLine<T>::LibLine(LibPoint<T> ptOrigin, LibVector<T> ptDirection)
{
	m_ptOrigin = ptOrigin;
	m_ptDirection = ptDirection;
}

template<typename T>
T LibLine<T>::AngleBetweenLines(const LibLine& lnOther) const
{
	return m_ptDirection.AngleBetweenVec(lnOther.m_ptDirection);;
}

template<typename T>
bool LibLine<T>::IsParallel(const LibLine& lnOther) const
{
	return m_ptDirection.IsCollinear(lnOther.m_ptDirection);
}

template<typename T>
bool LibLine<T>::IsOrtogonal(const LibLine& lnOther) const
{
	return m_ptDirection.IsOrtogonal(lnOther.m_ptDirection);;
}

template<typename T>
bool LibLine<T>::IsOpposite(const LibLine& lnOther) const
{
	return m_ptDirection.IsOpposite(lnOther.m_ptDirection);;
}

template<typename T>
bool LibLine<T>::IsSkew(const LibLine<T>& lnOther) const
{
	auto inters = IsIntersectionLine(lnOther);
	return !IsParallel(lnOther) && std::holds_alternative<bool>(inters);
}

template<typename T>
std::variant<LibPoint<T>, bool> LibLine<T>::IsIntersectionLine(const LibLine& lnOther) const
{
	if (IsParallel(lnOther)) {
		return false;
	}

	LibVector<T> vecCross = m_ptDirection.CrossProduct(lnOther.m_ptDirection);

	LibVector<T> vec = LibVector<T>(LibPoint<T>(m_ptOrigin.X() - lnOther.m_ptOrigin.X(),
		m_ptOrigin.Y() - lnOther.m_ptOrigin.Y(),
		m_ptOrigin.Z() - lnOther.m_ptOrigin.Z()));

	double coef1 = vec.CrossProduct(lnOther.m_ptDirection) / vecCross;
	double coef2 = vec.CrossProduct(m_ptDirection) / vecCross;

	LibVector<T> inters1 = LibVector(m_ptOrigin) + coef1 * m_ptDirection;
	LibVector<T> inters2 = LibVector(lnOther.m_ptOrigin) + coef2 * lnOther.m_ptDirection;

	if (std::fabs(inters1.X() - inters2.X()) <= 1e-9 &&
		std::fabs(inters1.Y() - inters2.Y())<= 1e-9 &&
		std::fabs(inters1.Z() - inters2.Z()) <= 1e-9) {
		return LibPoint<T>(inters1.X(), inters1.Y(), inters1.Z());
	}
	return false;
}

template<typename T>
LibPoint<T> LibLine<T>::ClosestPointOnLine(const LibPoint<T>& point) const
{
	LibVector<T> vec = LibVector<T>(point - m_ptOrigin);
	double coef = vec.DotProduct(m_ptDirection.Normalize());
	LibVector<T> scalDir = coef * m_ptDirection;
	return m_ptOrigin + scalDir.Coordinates();
}

template<typename T>
bool LibLine<T>::IsPointOnLine(const LibPoint<T>& point) const
{
	LibVector<T> vec = LibVector<T>(point - m_ptOrigin);

	if (m_ptDirection.LengthVector() <= 1e-9) {
		return false;
	}

	vec.Normalize();

	return vec.IsZero() || std::fabs(vec.DotProduct(m_ptDirection) - 1) <= 1e-9;
}

template<typename T>
double LibLine<T>::DistBetweenLines(const LibLine<T>& lnOther) const
{
	if (std::holds_alternative<LibPoint<T>>(IsIntersectionLine(lnOther))) {
		return 0;
	}
	if (IsParallel(lnOther)) {
		LibPoint<T> closestPoint = ClosestPointOnLine(lnOther.Origin());
		LibVector<T> dist = LibVector<T>(m_ptOrigin - closestPoint);
		return dist.LengthVector();
	}

	LibVector<T> vec = LibVector<T>(lnOther.Origin() - m_ptOrigin);
	LibVector<T> vecCross = m_ptDirection.CrossProduct(lnOther.m_ptDirection);


	return vec.DotProduct(vecCross) / vecCross.LengthVector();
}
