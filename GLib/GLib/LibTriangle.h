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
		LibPlane<T> pln(m_PtFrst, GetNormalTrgngl());
		if (pln.IsPointOnPlane(pt)) {
			T fstArea, fsptArea, ftptArea, tsptArea;
			GetAllArea(pt, fstArea, fsptArea, ftptArea, tsptArea);

			return LibEps<T>::IsZero(fsptArea + ftptArea + tsptArea - fstArea);
		}

		return false;
	}

	bool GetParamByPoint(const LibPoint<T>& pt, double& prmU, double& prmV) const {
		if (!IsPointOnTrngl(pt)) {
			return false;
		}

		T fstArea, fsptArea, ftptArea, tsptArea;
		GetAllArea(pt, fstArea, fsptArea, ftptArea, tsptArea);

		prmU = fsptArea / fstArea;
		prmV = tsptArea / fstArea;
		return true;
	}

	LibPoint<T> GetPointByParam(const double prmU, const double prmV) const {
		return (1 - prmU - prmV) * m_PtFrst + prmU * m_PtScnd + prmV * m_PtThrd;
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