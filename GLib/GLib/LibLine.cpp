//#include "LibLine.h"
//#include <cmath>
//
//template<typename T>
//LibLine<T>::LibLine(const LibPoint<T>& ptOrigin, const LibVector<T>& ptDirection)
//{
//	m_ptOrigin = ptOrigin;
//	m_ptDirection = ptDirection;
//}
//
//template<typename T>
//T LibLine<T>::AngleBetweenLines(const LibLine& lnOther) const
//{
//	return m_ptDirection.AngleBetweenVec(lnOther.m_ptDirection);
//}
//
//template<typename T>
//bool LibLine<T>::IsParallel(const LibLine& lnOther) const
//{
//	return m_ptDirection.IsParallel(lnOther.m_ptDirection);
//}
//
//template<typename T>
//bool LibLine<T>::IsOrtogonal(const LibLine& lnOther) const
//{
//	return m_ptDirection.IsOrtogonal(lnOther.m_ptDirection);;
//}
//
//template<typename T>
//bool LibLine<T>::IsOpposite(const LibLine& lnOther) const
//{
//	return m_ptDirection.IsOpposite(lnOther.m_ptDirection);;
//}
//
//template<typename T>
//bool LibLine<T>::IsSkew(const LibLine<T>& lnOther) const
//{
//	bool inters = IsIntersectionLine(lnOther);
//	return !IsParallel(lnOther) && !inters;
//}
//
//template<typename T>
//bool LibLine<T>::GetIntersParam(const LibLine<T>& lnOther, T& coef1, T& coef2) const
//{
//	if (IsParallel(lnOther)) {
//		return false;
//	}
//
//	LibVector<T> vecCross = m_ptDirection.CrossProduct(lnOther.m_ptDirection);
//
//	LibVector<T> vec = m_ptOrigin - lnOther.m_ptOrigin;
//
//	coef1 = vec.CrossProduct(m_ptDirection) / vecCross;
//	coef2 = vec.CrossProduct(lnOther.Direction()) / vecCross;
//	return true;
//}
//
//template<typename T>
//bool LibLine<T>::GetIntersection(double coef, LibPoint<T>& intersPoint)
//{
//	intersPoint = m_ptOrigin + coef * m_ptDirection;
//	return true;
//}
//
//template<typename T>
//bool LibLine<T>::IsIntersectionLine(const LibLine& lnOther, LibPoint<T>& intersPoint) const
//{
//	T	 coef1, coef2;
//	if (GetIntersParam(lnOther, coef1, coef2))
//	{
//		return GetIntersection(coef2);
//	}
//	return false;
//}
//
//template<typename T>
//LibPoint<T> LibLine<T>::ClosestPointOnLine(const LibPoint<T>& point) const
//{
//	LibVector<T> vec = LibVector<T>(point - m_ptOrigin);
//	double coef = vec.DotProduct(m_ptDirection.Normalize());
//	LibVector<T> scalDir = coef * m_ptDirection;
//	return m_ptOrigin + scalDir.Coordinates();
//}
//
//template<typename T>
//bool LibLine<T>::IsPointOnLine(const LibPoint<T>& point, double eps) const
//{
//	LibVector<T> vec = LibVector<T>(point - m_ptOrigin);
//
//	if (m_ptDirection.LengthVector() <= LibEps::eps) {
//		return false;
//	}
//
//	return LibEps::IsZero(std::fabs(vec.DotProduct(m_ptDirection) - 1), eps);
//}
//
//template<typename T>
//double LibLine<T>::DistBetweenLines(const LibLine<T>& lnOther) const
//{
//	if (IsIntersectionLine(lnOther)) {
//		return 0;
//	}
//	if (IsParallel(lnOther)) {
//		LibPoint<T> closestPoint = ClosestPointOnLine(lnOther.Origin());
//		LibVector<T> dist = LibVector<T>(m_ptOrigin - closestPoint);
//		return dist.LengthVector();
//	}
//
//	//LibVector<T> vec = LibVector<T>(lnOther.Origin() - m_ptOrigin);
//	//LibVector<T> vecCross = m_ptDirection.CrossProduct(lnOther.m_ptDirection);
//
//	return vec.DotProduct(vecCross) / vecCross.LengthVector();
//}
