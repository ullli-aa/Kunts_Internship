#include "LibRay.h"

#include <cmath>

template<typename T>
std::variant<LibPoint<T>, bool> LibRay<T>::IsIntersectionLine(const LibLine<T>& lnOther) const
{
	if (IsParallel(lnOther)) {
		return false;
	}

	LibVector<T> vec = LibVector<T>(LibPoint<T>(this->m_ptOrigin.X() - lnOther.Origin().X(),
		this->m_ptOrigin.Y() - lnOther.Origin().Y(),
		this->m_ptOrigin.Z() - lnOther.Origin().Z()));

	LibVector<T> vecCross = this->m_ptDirection.CrossProduct(lnOther.Direction());

	double coefR = vec.CrossProduct(this->m_ptDirection) / vecCross;
	double coefL = vec.CrossProduct(lnOther.Direction()) / vecCross;

	if (coefR < 0) {
		return false;
	}

	LibVector<T> inters1 = LibVector(this->m_ptOrigin) + coefR * this->m_ptDirection;
	LibVector<T> inters2 = LibVector(lnOther.Origin()) + coefL * lnOther->Direction();

	if (std::fabs(inters1.X() - inters2.X()) <= 1e-9 &&
		std::fabs(inters1.Y() - inters2.Y()) <= 1e-9 &&
		std::fabs(inters1.Z() - inters2.Z()) <= 1e-9) {
		return LibPoint<T>(inters1.X(), inters1.Y(), inters1.Z());
	}
	return false;
}

template<typename T>
std::variant<LibPoint<T>, bool> LibRay<T>::IsIntersectionRay(const LibRay<T>& rayOther) const
{
	if (IsParallel(rayOther)) {
		return false;
	}

	LibVector<T> vecCrossd1d2 = this->m_ptDirection.CrossProduct(rayOther.m_ptDirection);
	LibVector<T> vecCrossd2d1 = rayOther.m_ptDirection.CrossProduct(this->m_ptDirection);

	LibVector<T> vec = LibVector<T>(LibPoint<T>(this->m_ptOrigin.X() - rayOther.m_ptOrigin.X(),
		this->m_ptOrigin.Y() - rayOther.m_ptOrigin.Y(),
		this->m_ptOrigin.Z() - rayOther.m_ptOrigin.Z()));

	double denom1 = this->m_ptDirection.DotProduct(vecCrossd2d1);
	double denom2 = rayOther.m_ptDirection.DotProduct(vecCrossd1d2);

	if (std::fabs(denom1) <= 1e-9 || std::fabs(denom2) <= 1e-9) {
		return false;
	}

	double coef1 = vec.DotProduct(vecCrossd2d1) / denom1;
	double coef2 = vec.DotProduct(vecCrossd1d2) / denom2;

	if (coef1 >= 0 && coef2 >= 0) {
		LibPoint<T> inters1 = this->m_ptOrigin + coef1 * this->m_ptDirection.Coordinates();
		LibPoint<T> inters2 = rayOther.m_ptOrigin + coef2 * rayOther.m_ptDirection.Coordinates();

		if (std::fabs(inters1.X() - inters2.X()) <= 1e-9 &&
			std::fabs(inters1.Y() - inters2.Y()) <= 1e-9 &&
			std::fabs(inters1.Z() - inters2.Z()) <= 1e-9) {
			return LibPoint<T>(inters1.X(), inters1.Y(), inters1.Z());
		}
	}
	return false;
}

template<typename T>
LibPoint<T> LibRay<T>::ClosestPointOnLine(const LibPoint<T>& point) const
{
	LibVector<T> vec = LibVector<T>(point - this->m_ptOrigin);
	this->m_ptDirection.Normalize();

	double projectionLength = vec.DotProduct(this->m_ptDirection);
	if (projectionLength < 0) {
		return this->m_ptOrigin;
	}
	LibVector<T> scalDir = projectionLength * this->m_ptDirection;
	return this->m_ptOrigin + scalDir.Coordinates();
}

template<typename T>
bool LibRay<T>::IsPointOnLine(const LibPoint<T>& point) const
{
	LibVector<T> vec = LibVector<T>(point - this->m_ptOrigin);
	return !vec.IsOpposite(this->Direction) && vec.LengthVector >= 0;
}