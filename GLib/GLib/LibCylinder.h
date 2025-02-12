#pragma once

#include "LibPoint.h"
#include "LibVector.h"
#include "LibLine.h"
#include "LibEps.h"
#include "LibUtility.h"

template<typename T>
class LibCylinder {
public:
	LibCylinder() = default;
	LibCylinder(const LibPoint<T>& pt, const LibVector<T>& dir, T radius) :
		m_ptStartPt(pt), m_vecDirection(dir.GetNormalize()), m_TRadius(radius) {};
	~LibCylinder() = default;

	inline const LibPoint<T>& StartPoint() const {
		return m_ptStartPt;
	}

	inline const  LibVector<T>& Direction() const {
		return m_vecDirection;
	}

	inline T Radius() const {
		return radius;
	}

	inline LibCylinder<T>& SetStart(const LibPoint<T>& pt) {
		m_ptStartPt = pt;
		return *this;
	}

	inline LibCylinder<T>& SetDirection(const LibVector<T>& dir) {
		m_vecDirection = dir;
		return *this;
	}

	inline LibCylinder<T>& SetRadius(const T radius) {
		m_TRadius = radius;
		return *this;
	}

	bool IsIntersectionLine(const LibLine<T>& line, LibPoint<T>& intersPoint) const {
		//TODO
	}

	bool GetParamByPoint(const LibPoint<T>& pt, double& prmU, double& prmV) const {
		//TODO
	}

	LibPoint<T> GetPointByParam(const double prmU, const double prmV) const {
		//TODO
	}

	bool GetNormalInPt(const LibPoint<T>& pt, LibVector<T>& normal) const {
		if (!IsPointOnCylinder(pt)) {
			return false;
		}

		LibPoint<T> ptClosestToAxis = GetClosestAxisPoint(pt);
		normal = pt - ptClosestToAxis;
		normal.NormalizeThis();
		return true;
	}

	bool IsPointOnCylinder(const LibPoint<T>& pt, double eps = Lib::Eps) const {
		LibPoint<T> ptClosestToAxis = GetClosestAxisPoint(pt);
		return LibEps::IsZero((pt - ptClosestToAxis).LengthVector() - Radius());
	}

	LibPoint<T> PointProjection(const LibPoint<T>& pt) const {
		if (IsPointOnCylinder(pt)) {
			return pt;
		}

		LibVector<T> ClosestToPt;
		if (GetNormalInPt(ClosestToPt)) {
			return ptClosestToAxis + ClosestToPt * Radius();
		}
	}

private:
	LibPoint<T> GetClosestAxisPoint(LibPoint<T> pt) {
		LibLine<T> axis(StartPoint(), Direction());
		return ptClosestToAxis = axis.ClosestPointOnLine(pt);
	}

	LibPoint<T> m_ptStartPt;
	LibVector<T> m_vecDirection;
	T m_TRadius;
};