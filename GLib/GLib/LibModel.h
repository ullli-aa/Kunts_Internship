#pragma once

#include <vector>
#include "LibPoint.h"
#include "LibVector.h"
#include "LibTriangle.h"
#include "LibRay.h"

template<typename T>
class LibModel
{
public:
	class Surface
	{
	public:
		Surface() = default;

		Surface(const LibModel<T>& model, size_t begin, size_t end) :
		m_model(model), m_begin(begin), m_end(end) { }

		inline LibModel<T> Model() const
		{
			return m_model;
		}

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
				(m_end == other.m_end) &&
				(&m_model == &other.m_model);
		}

	private:
		const LibModel<T>& m_model;
		size_t m_begin;
		size_t m_end;
	};

	LibModel() = default;

	LibModel(const std::vector<LibPoint<T>>& pts, const std::vector<LibVector<T>>& normals,
		const std::vector<int>& triangles, std::vector<Surface> surfaces) :
		m_vecPoints(pts), m_vecNormals(normals), m_vecTriangles(triangles), m_vecSurfaces(surfaces) {}

	~LibModel() = default;

	inline std::vector<LibPoint<T>> Points() const
	{
		return m_vecPoints;
	}

	inline std::vector<LibVector<T>> Normals() const
	{
		return m_vecNormals;
	}

	inline std::vector<int> Triangles() const
	{
		return m_vecTriangles;
	}

	inline std::vector<Surface> Surfaces() const
	{
		return m_vecSurfaces;
	}

	void CreateCube(LibPoint<T> center, T length)
	{
		T x = center.X(); T y = center.Y(); T z = center.Z();
		T halfLen = length / 2;

		LibPoint<T> A(x + halfLen, y - halfLen, z - halfLen);
		LibPoint<T> B(x + halfLen, y - halfLen, z + halfLen);
		LibPoint<T> C(x + halfLen, y + halfLen, z + halfLen);
		LibPoint<T> D(x + halfLen, y + halfLen, z - halfLen);
		LibPoint<T> G(x - halfLen, y - halfLen, z - halfLen);
		LibPoint<T> E(x - halfLen, y - halfLen, z + halfLen);
		LibPoint<T> F(x - halfLen, y + halfLen, z + halfLen);
		LibPoint<T> K(x - halfLen, y + halfLen, z - halfLen);

		m_vecPoints = std::vector<LibPoint<T>>{ A, D, C, B, D, K, F, C, K, G, E, F, G, A, B, E, B, C, F, E, G, K, D, A };

		LibVector<T> AB = B - A;
		LibVector<T> DA = A - D;
		LibVector<T> KD = D - K;
		LibVector<T> FK = K - F;
		LibVector<T> EF = F - E;
		LibVector<T> EB = B - E;

		LibVector<T> nrml1 = AB.CrossProduct(DA);
		LibVector<T> nrml2 = KD.CrossProduct(FK);
		LibVector<T> nrml3 = EF.CrossProduct(FK);
		LibVector<T> nrml4 = AB.CrossProduct(EB);
		LibVector<T> nrml5 = EB.CrossProduct(EF);
		LibVector<T> nrml6 = DA.CrossProduct(KD);
		m_vecNormals = std::vector<LibVector<T>>{ nrml1, nrml1, nrml1, nrml1, nrml2, nrml2, nrml2, nrml2,
						nrml3, nrml3, nrml3, nrml3, nrml4, nrml4, nrml4, nrml4,
						nrml5, nrml5, nrml5, nrml5, nrml6, nrml6, nrml6, nrml6 };

		m_vecSurfaces.clear();
		for (size_t i = 0; i < m_vecPoints.size(); i += 4)
		{
			m_vecSurfaces.push_back(Surface(*this, i, i + 4));
		}

		m_vecTriangles.clear();
		for (auto& surface : m_vecSurfaces)
		{
			for (size_t i = surface.Begin() + 1; i < surface.End() - 1; i++)
			{
				m_vecTriangles.push_back(surface.Begin());
				m_vecTriangles.push_back(i);
				m_vecTriangles.push_back(i + 1);
			}
		}
	}

	bool IsIntersectionRay(const LibRay<T>& ray, LibPoint<T>& pt, Surface& srfc) const {
		T dist = DBL_MAX;
		int ind = 0;
		LibPoint<T> intersPt;
		for (size_t i = 0; i < m_vecTriangles.size(); i += 3) {
			LibTriangle<T> trngl(m_vecPoints[m_vecTriangles[i]],
				m_vecPoints[m_vecTriangles[i + 1]],
				m_vecPoints[m_vecTriangles[i + 2]]);
			if (trngl.IsIntersectionLine(ray, intersPt)) {
				T curDist = intersPt.DistanceTo(ray.Origin());
				if (curDist < dist) {
					dist = curDist;
					ind = i / 3 + 1;
					pt = intersPt;
				}
			}
		}

		srfc = FindSurfForTrngl(ind);
	}
	
private:
	Surface& FindSurfForTrngl(int ind) const {
		for (const Surface& surf : m_vecSurfaces) {
			if (surf.End() - surf.Begin() < ind) {
				ind -= surf.End() - surf.Begin();
			}
			else {
				return surf;
			}
		}
	}

	std::vector<LibPoint<T>> m_vecPoints;
	std::vector<LibVector<T>> m_vecNormals;
	std::vector<int> m_vecTriangles;
	std::vector<Surface> m_vecSurfaces;
};