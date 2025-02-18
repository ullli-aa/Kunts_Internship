#pragma once

#include "LibPoint.h"
#include "LibLine.h"
#include "Plane.h"
#include "LibEps.h"

template<typename T>
class LibTriangle {
public:
	LibTriangle() = default;
	LibTriangle(const LibPoint<T>& frst, const LibPoint<T>& scnd, const LibPoint<T>& thrd) :
		m_PtFrst(frst), m_PtScnd(scnd), m_PtThrd(thrd) {};
	~LibTriangle() = default;

	inline const LibPoint<T>& First() const {
		return m_PtFrst;
	}

	inline const LibPoint<T>& Second() const {
		return m_PtScnd;
	}

	inline const LibPoint<T>& Third() const {
		return m_PtThrd;
	}

	inline LibTriangle<T>& SetFirst(const LibPoint<T>& frst) {
		m_PtFrst = frst;
		return *this;
	}

	inline LibTriangle<T>& SetSecond(const LibPoint<T>& scnd) {
		m_PtScnd = scnd;
		return *this;
	}

	inline LibTriangle<T>& SetThird(const LibPoint<T>& thrd) {
		m_PtThrd = thrd;
		return *this;
	}

	LibVector<T> GetNormalTrgngl() const {
		LibVector<T> fs = m_PtScnd - m_PtFrst;
		LibVector<T> ft = m_PtThrd - m_PtFrst;
		return fs.CrossProduct(ft).NormalizeThis();
	}

	T Area() const {
		T fs = (m_PtScnd - m_PtFrst).LengthVector();
		T ft = (m_PtThrd - m_PtFrst).LengthVector();
		T ts = (m_PtScnd - m_PtThrd).LengthVector();

		T p = (fs + ft + ts) / 2;

		//Geron
		return std::sqrt(p * (p - fs) * (p - ft) * (p - ts));
	}

	bool IsIntersectionLine(const LibLine<T>& line, LibPoint<T>& pt) const {
		LibPlane<T> pln(m_PtFrst, GetNormalTrgngl());
		if (pln.IsIntersectionLine(line, pt)) {
			return IsPointOnTrngl(pt);
		}

		return false;
	}

	bool IsPointOnTrngl(const LibPoint<T>& pt) const {
		double u, v;
		if (!GetParamByPoint(pt, u, v)) {
			return false;
		}
		return u >= 0 && v >= 0 && (1 - u - v) >= 0;
	}

	bool GetParamByPoint(const LibPoint<T>& pt, double& prmU, double& prmV) const {
		LibVector<T> AB = m_PtScnd - m_PtFrst;
		LibVector<T> AC = m_PtThrd - m_PtFrst;
		LibVector<T> AP = pt - m_PtFrst;

		LibVector<T> normal = AB.CrossProduct(AC);
		if (normal.IsParallel(AP.CrossProduct(AB))) {
			prmU = normal.DotProduct(AP.CrossProduct(AC)) / normal.DotProduct(normal);
			prmV = normal.DotProduct(AB.CrossProduct(AP)) / normal.DotProduct(normal);
			return prmU >= 0 && prmV >= 0 && (1 - prmU - prmV) >= 0;;
		}
		return false;
	}

	LibPoint<T> GetPointByParam(const double prmU, const double prmV) const {
		return (1 - prmU - prmV) * m_PtFrst.AsVector() + prmU * m_PtScnd.AsVector() + prmV * m_PtThrd.AsVector();
	}

private:
	void GetAllArea(const LibPoint<T>& pt, T& fstArea, T& fsptArea, T& ftptArea, T& tsptArea) const {
		fstArea = Area();
		fsptArea = LibTriangle(m_PtFrst, m_PtScnd, pt).Area();
		ftptArea = LibTriangle(m_PtFrst, m_PtThrd, pt).Area();
		tsptArea = LibTriangle(m_PtThrd, m_PtScnd, pt).Area();
	}

	LibPoint<T> m_PtFrst;
	LibPoint<T> m_PtScnd;
	LibPoint<T> m_PtThrd;
};