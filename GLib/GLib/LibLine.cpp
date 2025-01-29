#include "LibLine.h"

template<typename T>
LibLine<T>::LibLine(LibPoint<T> ptOrigin, LibVector<T> ptDirection)
{
	m_ptOrigin = origin;
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
bool LibLine<T>::IsIntersection(const LibLine& lnOther) const
{
	if (IsParallel(lnOther)) {
		return false;
	}
	LibVector<T> vecCrossd1d2 = m_ptDirection.CrossProduct(lnOther.m_ptDirection);
	LibVector<T> vecCrossd2d1 = lnOther.m_ptDirection.CrossProduct(m_ptDirection);
	
	LibVector<T> vec = LibVector<T>(LibPoint<T>(m_ptOrigin.X - lnOther.m_ptOrigin.X,
										   	    m_ptOrigin.Y - lnOther.m_ptOrigin.Y,
											    m_ptOrigin.Z - lnOther.m_ptOrigin.Z);

	double coef1 = vec.DotProduct(vecCrossd2d1) / (m_ptDirection.DotProduct(vecCrossd2d1);
	double coef2 = vec.DotProduct(vecCrossd1d2) / (lnOther.m_ptDirection.DotProduct(vecCrossd1d2);

	LibVector<T> inters1 = LibVector(m_ptOrigin) + coef1 * m_ptDirection;
	LibVector<T> inters2 = LibVector(lnOther.m_ptOrigin) + coef1 * lnOther.m_ptDirection;

	return std::fabs(inters1.X - inters2.X <= 1e-9) &&
		   std::fabs(inters1.Y - inters2.Y <= 1e-9) &&
		   std::fabs(inters1.Z - inters2.Z <= 1e-9);
}
