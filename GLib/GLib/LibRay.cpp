//#include "LibRay.h"
//
//#include <cmath>
//
//template<typename T>
//bool LibRay<T>::IsIntersectionLine(const LibLine<T>& lnOther, LibPoint<T>& intersPoint) const
//{
//	T coefR, coefL;
//	if (GetIntersParam(lnOther, coefR, coefL))
//	{
//		if (coefR < 0) {
//			return false;
//		}
//		return GetIntersection(coefR);
//	}
//	return false;
//}
//
//template<typename T>
//bool LibRay<T>::IsIntersectionRay(const LibRay<T>& rayOther, LibPoint<T>& intersPoint) const
//{
//	T coef1, coef2;
//	if (GetIntersParam(lnOther, coefR, coefL))
//	{
//		if (coef1 >= 0 && coef2 >= 0) {
//			return GetIntersection(coef2);
//		}
//	}
//	return false;
//}
//
//template<typename T>
//LibPoint<T> LibRay<T>::ClosestPointOnLine(const LibPoint<T>& point) const
//{
//	LibVector<T> vec = LibVector<T>(point - this->m_ptOrigin);
//	this->m_ptDirection.Normalize();
//
//	double projectionLength = vec.DotProduct(this->m_ptDirection);
//	if (projectionLength < 0) {
//		return this->m_ptOrigin;
//	}
//	LibVector<T> scalDir = projectionLength * this->m_ptDirection;
//	return this->m_ptOrigin + scalDir.Coordinates();
//}
//
//template<typename T>
//bool LibRay<T>::IsPointOnLine(const LibPoint<T>& point) const
//{
//	LibVector<T> vec = LibVector<T>(point - this->m_ptOrigin);
//	return !vec.IsOpposite(this->Direction) && vec.LengthVector >= 0;
//}