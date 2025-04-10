#pragma once

#include <vector>
#include <thread>
#include "LibPoint.h"
#include "LibVector.h"
#include "LibTriangle.h"
#include "LibRay.h"
#include "LibTimer.h"
#include "LibThreadPool.h"
#include "LibMatrix.h"
#include "LibCylinder.h"

template<typename T>
class LibModel
{
public:
	class Surface
	{
	public:
		Surface() = default;

		Surface(size_t begin, size_t end) :
			 m_begin(begin), m_end(end) { }

		inline size_t Begin() const
		{
			return m_begin;
		}

		inline size_t End() const
		{
			return m_end;
		}

		bool operator==(const Surface& other) const
		{
			return (m_begin == other.m_begin) &&
				(m_end == other.m_end);
		}

		void Save(std::ostream& out) const {
			LibUtility::Save(out, m_begin);
			LibUtility::Save(out, m_end);
		}

		void Load(std::istream& in) {
			LibUtility::Load(in, m_begin);
			LibUtility::Load(in, m_end);
		}

	private:
		size_t m_begin;
		size_t m_end;
	};

	LibModel() = default;

	LibModel(const std::vector<LibPoint<T>>& pts, const std::vector<LibVector<T>>& normals,
		const std::vector<size_t>& triangles, const std::vector<Surface>& surfaces) :
		m_vecPoints(pts), m_vecNormals(normals), m_vecTriangles(triangles), m_vecSurfaces(surfaces) {}

	~LibModel() = default;

	inline size_t TrinaglesNum() const { return m_vecTriangles.size() / 3; }
	inline size_t GetPointIndex(size_t idxTriangle, size_t idxPoint) const { return m_vecTriangles[idxTriangle * 3 + idxPoint]; }
	inline size_t GetPointIndex(const Surface& srf, size_t idxTriangle, size_t idxPoint) const { return m_vecTriangles[srf.Begin() * 3 + idxTriangle * 3 + idxPoint]; }

	inline const LibPoint<T>& GetPtInTrngl(size_t idxTriangle, size_t pos) const {
		return m_vecPoints[GetPointIndex(idxTriangle, pos)];
	}

	inline const LibPoint<T>& GetPtInTrngl(const Surface& srf, size_t idxTriangle, size_t pos) const {
		return m_vecPoints[GetPointIndex(srf, idxTriangle, pos)];
	}

	const LibVector<T>& GetNrmlsInTrngl(size_t idxTriangle, size_t pos) const {
		return m_vecNormals[GetPointIndex(idxTriangle, pos)];
	}

	const LibVector<T>& GetNrmlsInTrngl(const Surface& srf, size_t idxTriangle, size_t pos) const {
		return m_vecNormals[GetPointIndex(srf, idxTriangle, pos)];
	}

	inline const std::vector<LibPoint<T>>& Points() const
	{
		return m_vecPoints;
	}

	inline const std::vector<LibVector<T>>& Normals() const
	{
		return m_vecNormals;
	}

	inline const std::vector<size_t>& Triangles() const
	{
		return m_vecTriangles;
	}

	inline const std::vector<Surface>& Surfaces() const
	{
		return m_vecSurfaces;
	}

	inline void SetPoints(const std::vector<LibPoint<T>>& pts)
	{
		m_vecPoints = pts;
	}

	inline void SetNormals(const std::vector<LibVector<T>>& nrmls)
	{
		m_vecNormals = nrmls;
	}

	inline void SetTriangles(const std::vector<size_t>& trngls)
	{
		m_vecTriangles = trngls;
	}

	inline void SetSurfaces(const std::vector<Surface>& srfc)
	{
		m_vecSurfaces = srfc;
	}

	inline void SetModel(const LibModel<T> mdl) {
		Clear();
		SetPoints(mdl.Points());
		SetNormals(mdl.Normals());
		SetTriangles(mdl.Triangles());
		SetSurfaces(mdl.Surfaces());
	}

	bool operator==(const LibModel<T>& other) const {
		return Points() == other.Points() && Normals() == other.Normals() &&
			Triangles() == other.Triangles() && Surfaces() == other.Surfaces();
	}

	void Clear() {
		m_vecPoints.clear();
		m_vecNormals.clear();
		m_vecTriangles.clear();
		m_vecSurfaces.clear();
	}

	LibVector<T> Diagonal() const {
		LibPoint<T> minPoint = { std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max() };
		LibPoint<T> maxPoint = { std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest() };

		for (const auto& point : Points()) {
			if (point.X() < minPoint.X()) {
				minPoint.SetX(point.X());
			}
			if (point.Y() < minPoint.Y()) {
				minPoint.SetY(point.Y());
			}
			if (point.Z() < minPoint.Z()) {
				minPoint.SetZ(point.Z());
			}

			if (point.X() > maxPoint.X()) {
				maxPoint.SetX(point.X());
			}
			if (point.Y() > maxPoint.Y()) {
				maxPoint.SetY(point.Y());
			}
			if (point.Z() > maxPoint.Z()) {
				maxPoint.SetZ(point.Z());
			}
		}
		return maxPoint - minPoint;
	};


	static LibModel<T> CreateCube(const LibPoint<T>& center, T length)
	{
		T x = center.X(); T y = center.Y(); T z = center.Z();
		T halfLen = length / 2;

		LibPoint<T> A(x - halfLen, y - halfLen, z - halfLen);
		LibPoint<T> B(x + halfLen, y - halfLen, z - halfLen);
		LibPoint<T> C(x + halfLen, y + halfLen, z - halfLen);
		LibPoint<T> D(x - halfLen, y + halfLen, z - halfLen);
		LibPoint<T> G(x - halfLen, y - halfLen, z + halfLen);
		LibPoint<T> E(x + halfLen, y - halfLen, z + halfLen);
		LibPoint<T> F(x + halfLen, y + halfLen, z + halfLen);
		LibPoint<T> K(x - halfLen, y + halfLen, z + halfLen);

		std::vector<LibPoint<T>> vecPoints = std::vector<LibPoint<T>>{ A, D, C, B, G, E, F, K, A, B, E, G, D, K, F, C, A, G, K, D, C, F, E, B };
		LibVector<T> FK = K - F;
		LibVector<T> EF = F - E;
		LibVector<T> CF = F - C;

		LibVector<T> nrml1 = EF.CrossProduct(FK);
		LibVector<T> nrml2 = CF.CrossProduct(EF) * (-1);
		LibVector<T> nrml3 = CF.CrossProduct(FK) * (-1);

		std::vector<LibVector<T>> vecNormals = std::vector<LibVector<T>>{
						nrml1* (-1), nrml1* (-1), nrml1* (-1), nrml1* (-1),
						nrml1, nrml1, nrml1, nrml1,
						nrml2* (-1), nrml2* (-1), nrml2* (-1), nrml2* (-1),
						nrml2, nrml2, nrml2, nrml2,
						nrml3* (-1), nrml3* (-1), nrml3* (-1), nrml3* (-1),
						nrml3, nrml3, nrml3, nrml3
						};

		std::vector<Surface> vecSurfaces = {Surface(0, 2), Surface(2, 4), Surface(4, 6),
						Surface(6, 8), Surface(8, 10),Surface(10, 12) };

		std::vector<size_t> vecTriangles;
		vecTriangles.resize(6 * 2 * 3);
		int ind = 0;
		for (size_t i = 0; i < vecPoints.size(); i += 4)
		{
			vecTriangles[ind++] = i;
			vecTriangles[ind++] = i + 1;
			vecTriangles[ind++] = i + 2;

			vecTriangles[ind++] = i;
			vecTriangles[ind++] = i + 2;
			vecTriangles[ind++] = i + 3;
		}

		LibModel<T> model(vecPoints, vecNormals, vecTriangles, vecSurfaces);
		return model;
	}

	static LibModel<T> CreateCylinder(const LibPoint<T>& pt_Origin,
		const LibVector<T>& vec_Direction, T Radius, T Height, T ChordTolerance) {
		std::vector<LibPoint<T>> vecPoints;
		std::vector<LibVector<T>> vecNormals;
		std::vector<size_t> vecTriangles;
		std::vector<Surface> vecSurfaces;

		T angle = std::acos((Radius - ChordTolerance) / Radius) * 2;

		GetCirclePoints(vecPoints, vecNormals, vecTriangles, vecSurfaces,
			pt_Origin, vec_Direction, Radius, angle, -1);

		LibVector<T> nrmlDirection = vec_Direction.GetNormalize();
		LibPoint<T> pt_UpOrigin = pt_Origin + nrmlDirection * Height;
		GetCirclePoints(vecPoints, vecNormals, vecTriangles, vecSurfaces, 
			pt_UpOrigin, vec_Direction, Radius, angle, 1);

		LibCylinder<T> cylndr(pt_Origin, vec_Direction, Radius);
		size_t pntsCountOnCrcl = (vecPoints.size()) / 2 - 1;
		size_t trnglCount = vecTriangles.size() / 3;

		for (size_t i = 1; i < pntsCountOnCrcl + 1; i++)
		{
			LibPoint<T> A = vecPoints[i];
			LibPoint<T> B = vecPoints[i % pntsCountOnCrcl + 1];
			LibPoint<T> C = vecPoints[i % pntsCountOnCrcl + pntsCountOnCrcl + 2];
			LibPoint<T> D = vecPoints[i + pntsCountOnCrcl + 1];
			vecPoints.push_back(A);
			vecPoints.push_back(B);
			vecPoints.push_back(C);
			vecPoints.push_back(D);

			LibVector<T> normal1 = (A - vecPoints[0]).GetNormalize();
			LibVector<T> normal2 = (B - vecPoints[pntsCountOnCrcl + 1]).GetNormalize();
			cylndr.GetNormalInPt(A, normal1);
			cylndr.GetNormalInPt(B, normal2);

			vecNormals.push_back(normal1);
			vecNormals.push_back(normal2);
			vecNormals.push_back(normal2);
			vecNormals.push_back(normal1);

			vecTriangles.push_back(i);
			vecTriangles.push_back(i % pntsCountOnCrcl + 1);
			vecTriangles.push_back(i % pntsCountOnCrcl + pntsCountOnCrcl + 2);

			vecTriangles.push_back(i);
			vecTriangles.push_back(i % pntsCountOnCrcl + pntsCountOnCrcl + 2);
			vecTriangles.push_back(i + pntsCountOnCrcl + 1);
		}
		vecSurfaces.push_back(Surface(trnglCount, vecTriangles.size() / 3));

		LibModel<T> model(vecPoints, vecNormals, vecTriangles, vecSurfaces);
		return model;
	}

	bool IsIntersectionRay(const LibRay<T>& ray, LibPoint<T>& pt, int& srfc) const {
		TIMER_START("intersection of model and ray");

		T dist = DBL_MAX;
		size_t ind = 0;
		for (size_t i = 0; i < m_vecTriangles.size(); i += 3) {
			LibTriangle<T> trngl(m_vecPoints[m_vecTriangles[i]],
				m_vecPoints[m_vecTriangles[i + 1]],
				m_vecPoints[m_vecTriangles[i + 2]]);

			LibPoint<T> IntersPt;
			if (trngl.IsIntersectionLine(ray, IntersPt)) {
				if (!ray.IsPointOnLine(IntersPt)) {
					continue;
				}

				T curDist = IntersPt.DistanceTo(ray.Origin());
				if (curDist < dist) {
					dist = curDist;
					ind = i / 3;
				}
			}
		}

		if (dist == DBL_MAX) {

			TIMER_END("intersection of model and ray");
			return false;
		}

		pt = ray.Origin() + dist * ray.Direction().GetNormalize();
		srfc = FindSurfForTrngl(ind);
		
		TIMER_END("intersection of model and ray");

		return true;
	}

	bool IsIntersectionRayThread(const LibRay<T>& ray, LibPoint<T>& pt, int& srfc) const {
		TIMER_START("intersection with thread of model and ray");

		auto processTriangles = [&](size_t trnglIndex, size_t count, T& minDist, size_t& minInd) {
			size_t startIndex = trnglIndex * 3;
			size_t endIndex = startIndex + count * 3;
			for (size_t i = startIndex; i < endIndex; i += 3) {
				LibTriangle<T> trngl(m_vecPoints[m_vecTriangles[i]],
					m_vecPoints[m_vecTriangles[i + 1]],
					m_vecPoints[m_vecTriangles[i + 2]]);

				LibPoint<T> localIntersPt;
				if (trngl.IsIntersectionLine(ray, localIntersPt)) {
					if (!ray.IsPointOnLine(localIntersPt)) {
						continue;
					}

					T curDist = localIntersPt.DistanceTo(ray.Origin());

					if (curDist < minDist) {
						minDist = curDist;
						minInd = i / 3;
					}
				}
			}
		};

		const size_t numThreads = std::thread::hardware_concurrency();

		std::vector<std::thread> threads;
		std::vector<T> minDists(numThreads, DBL_MAX);
		std::vector<size_t> minInd(numThreads, 0);

		const size_t trnglsCount = Triangles().size() / 3;
		std::vector<size_t> trnglsForThread(numThreads, trnglsCount / numThreads);
		const size_t trnglsRemainder = trnglsCount % numThreads;

		for (size_t i = 0; i < trnglsRemainder; ++i) {
			trnglsForThread[i]++;
		}

		size_t startInd = 0;
		for (size_t t = 0; t < numThreads; ++t) {
			size_t count = trnglsForThread[t];
			threads.emplace_back(processTriangles, startInd, count, std::ref(minDists[t]), std::ref(minInd[t]));
			startInd += count;
		}

		for (auto& thread : threads) {
			thread.join();
		}

		T minDist = minDists[0];
		size_t ind = 0;
		for (size_t i = 1; i < numThreads; i++)
		{
			if (minDists[i] < minDist) {
				minDist = minDists[i];
				ind = minInd[i];
			}
		}

		if (minDist == DBL_MAX) {
			TIMER_END("intersection with thread of model and ray");
			return false;
		}

		pt = ray.Origin() + minDist * ray.Direction().GetNormalize();
		srfc = FindSurfForTrngl(ind);
		TIMER_END("intersection with thread of model and ray");

		return true;
	}

	bool IsIntersectionRayTP(const LibRay<T>& ray, LibPoint<T>& pt, int& srfc,
		LibThreadPool& tp) {
		TIMER_START("intersection with ThreadPool of model and ray");
		const size_t trnglsCount = Triangles().size() / 3;

		size_t trnglsPerThread = static_cast<size_t>(trnglsCount * 0.01);
		size_t trnglsRemainder = trnglsCount % trnglsPerThread;

		size_t taskCnt = trnglsCount / trnglsPerThread;
		T minDist = DBL_MAX;
		size_t minInd = 0;

		std::mutex mtx;

		size_t startInd = 0;
		for (size_t i = 0; i < taskCnt; ++i) {
			size_t count = trnglsPerThread + (i < trnglsRemainder ? 1 : 0);
			tp.AddTask(std::make_unique<IntersectionTask>(*this, ray, startInd, count, minDist, minInd, mtx));
			
			startInd += count;
		}

		tp.WaitForFinish();

		if (minDist == std::numeric_limits<T>::max()) {
			TIMER_END("intersection with ThreadPool of model and ray");
			return false;
		}

		pt = ray.Origin() + minDist * ray.Direction().GetNormalize();
		srfc = FindSurfForTrngl(minInd);
		TIMER_END("intersection with ThreadPool of model and ray");

		return true;
	}

	void Save(std::ostream& out) const {
		LibUtility::SaveVec(out, m_vecPoints);

		LibUtility::SaveVec(out, m_vecNormals);

		LibUtility::SaveBuf(out, m_vecTriangles);

		LibUtility::SaveVec(out, m_vecSurfaces);
	}

	void Load(std::istream& in) {
		LibUtility::LoadVec(in, m_vecPoints);
		LibUtility::LoadVec(in, m_vecNormals);

		LibUtility::LoadBuf(in, m_vecTriangles);

		LibUtility::LoadVec(in, m_vecSurfaces);
	}
	
protected:
	int FindSurfForTrngl(size_t ind) const {
		int res = 0;
		for (const Surface& surf : m_vecSurfaces) {
			if (ind >= surf.Begin() && ind < surf.End()) {
				return res;
			}
			res++;
		}
	}

	static void GetCirclePoints(std::vector<LibPoint<T>>& vecPoints, std::vector<LibVector<T>>& vecNormals,
		std::vector<size_t>& vecTriangles, std::vector<Surface>& vecSurfaces,
		const LibPoint<T>& pt_Center, const LibVector<T>& vec_Direction,
		T Radius, T angle, T dirCoef)
	{
		int pntsCountOnCrcl = static_cast<int>((2 * M_PI) / angle);
		if ((2 * M_PI) / angle - pntsCountOnCrcl > 0.5) {
			pntsCountOnCrcl++;
		}

		size_t trnglsCount = vecTriangles.size() / 3;
		size_t pntsCount = vecPoints.size();

		LibMatrix<T> mtrx_Rotation = LibMatrix<T>::Rotation(vec_Direction, angle);
		LibVector<T> nrml = dirCoef * vec_Direction.GetNormalize();

		LibVector<T> CirclDir = vec_Direction.GetOrtogonalVec().GetNormalize();
		LibPoint<T> pt_OnCircle = pt_Center + CirclDir * Radius;

		vecPoints.push_back(pt_Center);
		vecNormals.push_back(nrml);
		for (int i = 0; i < pntsCountOnCrcl; i++)
		{
			pt_OnCircle = LibMatrix<T>::MultPt(pt_OnCircle, mtrx_Rotation);
			vecPoints.push_back(pt_OnCircle);
			vecNormals.push_back(nrml);
		}

		for (size_t i = 1; i <= pntsCountOnCrcl; ++i)
		{
			vecTriangles.push_back(pntsCount);
			if (dirCoef == -1) {
				vecTriangles.push_back(pntsCount + 1 + i % (pntsCountOnCrcl));
				vecTriangles.push_back(pntsCount + i);
			}
			else {
				vecTriangles.push_back(pntsCount + i);
				vecTriangles.push_back(pntsCount + 1 + i % (pntsCountOnCrcl));
			}
			
		}
		vecSurfaces.push_back(Surface(trnglsCount, vecTriangles.size() / 3));
	}

private:
	class IntersectionTask : public Task {
		const LibModel<T>& model;
		const LibRay<T>& ray;
		size_t startIndex;
		size_t count;
		T& minDist;
		size_t& minInd;
		std::mutex& mtx;

	public:
		IntersectionTask(const LibModel<T>& mdl, const LibRay<T>& r, size_t start, size_t cnt,
			T& minD, size_t& minI, std::mutex& mutex)
			: model(mdl), ray(r), startIndex(start), count(cnt),
			minDist(minD), minInd(minI), mtx(mutex) { }

		void Do() override {
			for (size_t i = startIndex * 3; i < (startIndex + count) * 3; i += 3) {
				LibTriangle<T> trngl(model.Points()[model.Triangles()[i]],
					model.Points()[model.Triangles()[i + 1]],
					model.Points()[model.Triangles()[i + 2]]);

				LibPoint<T> localIntersPt;
				if (trngl.IsIntersectionLine(ray, localIntersPt)) {
					if (!ray.IsPointOnLine(localIntersPt)) {
						continue;
					}

					T curDist = localIntersPt.DistanceTo(ray.Origin());

					mtx.lock();
					if (curDist < minDist) {
						minDist = curDist;
						minInd = i / 3;
					}
					mtx.unlock();
				}
			}
		}
	};

	std::vector<LibPoint<T>> m_vecPoints;
	std::vector<LibVector<T>> m_vecNormals;
	std::vector<size_t> m_vecTriangles;
	std::vector<Surface> m_vecSurfaces;
};
