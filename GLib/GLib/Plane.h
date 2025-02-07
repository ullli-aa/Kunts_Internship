#pragma once

#include "LibPoint.h"
#include "LibVector.h"
#include "LibLine.h"
#include "LibEps.h"

template<typename T>
class LibPlane {
public:
	LibPlane() = default;

	LibPLane(const LibPoint<T>& pt, const LibVector<T>& vec) :
		m_ptStartPt(pt), m_vecNormal(vec) {};

	~LibPlane() = default;

	inline const LibPoint<T>& StartPoint() const {
		return m_ptStartPt;
	}

	inline const LibVector<T>& Normal() const {
		return m_vecNormal;
	}

	inline LibPlane<T>& SetStartPt(const LibPoint<T>& pt) {
		m_ptStartPt = pt;
		return *this;
	}

	inline LibPlane<T>& SetNormal(const LibVector<T>& vec) {
		m_vecNormal = vec;
		return *this;
	}

	bool IsIntersectionLine(const LibLine<T>& lnOther, LibPoint<T>& intersPoint) const {
		if (Normal().IsOrtogonal(lnOther.Direction())) {
			return false;
		}

		LibVector<T> ptPlaneToPtLine = StartPoint() - lnOther.Origin();

		T param = Normal().DotProduct(ptPlaneToPtLine) / Normal().DotProduct(lnOther.Direction());
		intersPoint = lnOther.Origin() + param * lnOther.Direction();
		return true;
	}

	bool GetParamByPoint(const LibPoint<T>& pt, double& prmU, double& prmV) const {
		if (IsPointOnPlane(pt)) {
			LibVector<T> vecA = Normal().GetOrtogonalVec();
			LibVector<T> vecB = Normal().CrossProduct(vecA);

			LibVector<T> pt_startToPt = pt - StartPoint();

			prmU = pt_startToPt.DotProduct(vecA) / vecA.LengthVector();
			prmV = pt_startToPt.DotProduct(vecB) / vecB.LengthVector();
			return true;
		}
		return false;
	}

	LibPoint<T> GetPointByParam(const double prmU, const double prmV) const {
		LibVector<T> vecA = Normal().GetOrtogonalVec();
		LibVector<T> vecB = Normal().CrossProduct(vecA);

		return StartPoint() + vecA * prmU + vecB * prmV;
	}

	bool GetNormalInPt(const LibPoint<T>& pt, LibVector<T>& normal) const {
		if (IsPointOnPlane(pt)) {
			normal = m_vecNormal;
		}
		return false;
	}

	bool IsPointOnPlane(const LibPoint<T>& pt, double eps = Lib::Eps) const {
		LibVector<T> vec = pt - m_ptStartPt;
		return vec.IsOrtogonal(m_vecNormal, eps);
	}

	LibPoint<T> PointProjection(const LibPoint<T>& pt) const {
		if (IsPointOnPlane(pt)) {
			return pt;
		}

		LibVector<T> vec = pt - StartPoint();
		LibVector<T> projection = vec.ProjectionOnto(Normal());
		return pt - projection;
	}

	LibVector<T> VecProjection(const LibVector<T>& vec) const {
		return vec - vec.ProjectionOnto(Normal());
	}

private:
	LibPoint<T> m_ptStartPt;
	LibVector<T> m_vecNormal;
};