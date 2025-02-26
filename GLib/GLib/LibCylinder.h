#pragma once

#include "LibPoint.h"
#include "LibVector.h"
#include "LibLine.h"
#include "LibEps.h"
#include "LibUtility.h"
#include "LibMatrix.h"
#include "LibTimer.h"
#include <vector>

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
		return m_TRadius;
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

	std::vector<LibPoint<T>> IsIntersectionLine(const LibLine<T>& line) const {
		TIMER_START("intersection of cylinder and line");

		std::vector<LibPoint<T>> result;
		if (line.Direction().IsParallel(Direction())) {
			return result;
		}

		LibVector<T> n1, n2;
		GetCoordVec(n1, n2);

		LibMatrix<T> locCoord = LibMatrix<T>::ToCoordinatesInit(n1, n2, Direction(), StartPoint());

		LibVector<T> direction = line.Direction();
		LibVector<T> pt = LibMatrix<T>::MultVec(direction, locCoord);

		// line to loc coord
		LibLine<T> locLine = LibLine<T>(LibMatrix<T>::MultPt(line.Origin(), locCoord),
			pt);

		// look at OXY (x - x0)^2 + (y - y0)^2 = r^2
		T dx = locLine.Direction().X();
		T dy = locLine.Direction().Y();
		T dz = locLine.Direction().Z();
		T px = locLine.Origin().X();
		T py = locLine.Origin().Y();
		T pz = locLine.Origin().Z();

		T a = dx * dx + dy * dy;
		T b = 2 * (dx * px - dx * StartPoint().X() + dy * py - dy * StartPoint().Y());
		T c = (px - StartPoint().X()) * (px - StartPoint().X()) +
			(py - StartPoint().Y()) * (py - StartPoint().Y()) - Radius() * Radius();

		//count discriminant 
		T D = b * b - 4 * a * c;

		if (D < 0) {
			TIMER_END("intersection of cylinder and line");
			return result; // no inters
		}

		LibMatrix<T> globCoord = LibMatrix<T>::ToCoordinatesInit(
			LibVector<T>(1, 0, 0), LibVector<T>(0, 1, 0), LibVector<T>(0, 0, 1), LibPoint<T>(0, 0, 0));

		LibPoint<T> pt1, pt2;

		if (D == 0) {
			T t1 = -b / (2 * a);
			result.push_back(GetResForInters(t1, locLine, globCoord));
			TIMER_END("intersection of cylinder and line");
			return result;
		}

		T t1 = (-b + std::sqrt(D)) / (2 * a);
		result.push_back(GetResForInters(t1, locLine, globCoord));
		T t2 = (-b - std::sqrt(D)) / (2 * a);
		result.push_back(GetResForInters(t2, locLine, globCoord));

		TIMER_END("intersection of cylinder and line");

		return result;
	}

	bool GetParamByPoint(const LibPoint<T>& pt, double& prmU, double& prmV) const {
		if (!IsPointOnCylinder(pt)) {
			return false;
		}

		LibVector<T> n1, n2;
		GetCoordVec(n1, n2);

		LibMatrix<T> locCoord = LibMatrix<T>::ToCoordinatesInit(n1, n2, Direction(), StartPoint());
		LibPoint<T> locPt = LibMatrix<T>::MultPt(pt, locCoord);
		prmV = locPt.Z();
		prmU = std::atan2(locPt.Y(), locPt.X());
		return true;
	}

	LibPoint<T> GetPointByParam(const double prmU, const double prmV) const {
		LibVector<T> n1, n2;
		GetCoordVec(n1, n2);
		
		LibPoint<T> pt = StartPoint() + prmV * Direction() + Radius() * (std::cos(prmU) * n1 + std::sin(prmU) * n2);
		return pt;
	}

	bool GetNormalInPt(const LibPoint<T>& pt, LibVector<T>& normal) const {
		LibPoint<T> ptClosestToAxis = GetClosestAxisPoint(pt);
		normal = pt - ptClosestToAxis;
		normal.NormalizeThis();
		return true;
	}

	bool IsPointOnCylinder(const LibPoint<T>& pt, double eps = LibEps::eps) const {
		LibPoint<T> ptClosestToAxis = GetClosestAxisPoint(pt);
		return LibEps::IsZero((pt - ptClosestToAxis).LengthVector() - Radius());
	}

	LibPoint<T> PointProjection(const LibPoint<T>& pt) const {
		if (IsPointOnCylinder(pt)) {
			return pt;
		}

		LibVector<T> ClosestToPt;
		if (GetNormalInPt(ClosestToPt)) {
			return GetClosestAxisPoint(pt) + ClosestToPt * Radius();
		}
	}

private:
	LibPoint<T> GetClosestAxisPoint(const LibPoint<T>& pt) const {
		LibLine<T> axis(StartPoint(), Direction());
		return axis.ClosestPointOnLine(pt);
	}

	void GetCoordVec(LibVector<T>& n1, LibVector<T>& n2) const {
		n2 = Direction().GetOrtogonalVec();
		n1 = n2.CrossProduct(Direction());
	}

	LibPoint<T> GetResForInters(const T prm, const LibLine<T>& locLine, const LibMatrix<T>& globCoord) const {
		LibPoint<T> pt = locLine.Origin() + prm * locLine.Direction();
		return LibMatrix<T>::MultPt(pt, globCoord);
	}

	LibPoint<T> m_ptStartPt;
	LibVector<T> m_vecDirection;
	T m_TRadius;
};