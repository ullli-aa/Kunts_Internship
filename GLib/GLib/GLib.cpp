#include <fstream>

#include "MyTestMacros.h"
#include "LibPoint.h"
#include "LibVector.h"
#include "LibMatrix.h"
#include "LibCylinder.h"
#include "LibTriangle.h"
#include "LibLine.h"
#include "LibModel.h"
#include "LibRay.h"
#include "LibThreadPool.h"

typedef LibPoint<double> Pt;
typedef LibTriangle<double> Trngl;
typedef LibLine<double> Line;
typedef LibVector<double> Vec;
typedef LibMatrix<double> Mtrx;
typedef LibModel<double> Model;
typedef LibModel<double>::Surface Srfc;
typedef LibRay<double> Ray;
typedef LibThreadPool TP;

class Tests {
private:
	void PointTest_DistanceTo() {
		LibPoint<double> p1(1.0, 2.0, 3.0);
		MY_ASSERT_DOUBLE_EQ(p1.DistanceTo(p1), 0.0);
		MY_ASSERT_DOUBLE_EQ(p1.SquareDistanceTo(p1), 0.0);

		LibPoint<double> p2(3.0, 2.0, 1.0);
		MY_ASSERT_DOUBLE_EQ(p1.DistanceTo(p2), std::sqrt(8));
		MY_ASSERT_EQ(p2.SquareDistanceTo(p1), 8);

		p1 = LibPoint<double>(-1.0, -1.0);
		p2 = LibPoint<double>(1.0, 1.0);
		MY_ASSERT_DOUBLE_EQ(p2.DistanceTo(p1), std::sqrt(8));
		MY_ASSERT_EQ(p2.SquareDistanceTo(p1), 8);

		p1 = LibPoint<double>(-2.0, 1.0);
		p2 = LibPoint<double>(1.0, -2.0);
		MY_ASSERT_DOUBLE_EQ(p2.DistanceTo(p1), std::sqrt(18));
		MY_ASSERT_EQ(p2.SquareDistanceTo(p1), 18);

		p1 = LibPoint<double>(std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
		p2 = LibPoint<double>(-std::numeric_limits<double>::max(), -std::numeric_limits<double>::max());
		MY_ASSERT_DOUBLE_EQ(p2.DistanceTo(p1), std::sqrt(8) * std::numeric_limits<double>::max());
	}

	void PointTest_Operators()
	{
		LibPoint<double> p1(1.5, 2.3, 5.6);
		MY_ASSERT_TRUE(p1 == p1);

		LibPoint<double> p2(1.5 - 3e-5, 2.3, 5.6);
		LibPoint<double> p3(1.5 + 1e-5, 2.3, 5.6);
		MY_ASSERT_TRUE(p1 == p2);
		MY_ASSERT_FALSE(p1 != p3);
		MY_ASSERT_FALSE(p2 == p3);

		MY_ASSERT_TRUE(p3.IsEqual(p3));
		MY_ASSERT_FALSE(p1.IsEqual(p2, 1e-6));
		MY_ASSERT_TRUE(p2.IsEqual(p3, 1e-3));
	}

	void VecTest_Parallel()
	{
		LibVector<float> vec1(1.1f, 2.2f, 3.3f);
		LibVector<float> vec2(0.00001f, 0.00002f, 0.00003f);
		MY_ASSERT_TRUE(vec1.IsParallel(vec2));

		vec2.SetY(1e-5f);
		MY_ASSERT_FALSE(vec1.IsParallel(vec2, 1e-6));

		vec2.SetX(4).SetY(2e-5f);
		MY_ASSERT_FALSE(vec2.IsParallel(vec1, 1e-9));
	}

	void VecTest_IsZero()
	{
		LibVector<double> vec(0, 0, 0);
		MY_ASSERT_TRUE(vec.IsZero());

		vec = LibVector<double>(1e-9, 1e-9, 1e-9);
		MY_ASSERT_TRUE(vec.IsZero());

		vec = LibVector<double>(1e-5, 1e-9, 1e-9);
		MY_ASSERT_FALSE(vec.IsZero());
	}

	void VecTest_EqOperators()
	{
		LibVector<double> vec1(1.1, 2.2, 3.3);
		LibVector<double> vec2(1e-5, 2e-5, 3e-5);

		MY_ASSERT_FALSE(vec1 == vec2);

		vec2.SetY(1e-5);
		MY_ASSERT_FALSE(vec1.IsEqual(vec2, 1e-6));

		vec2.SetY(2e-5);
		MY_ASSERT_TRUE(vec1.IsEqual(vec2, 10));

		LibVector<double> vec3(1e-5, 1e-5, 1e-5);
		MY_ASSERT_TRUE(vec2 != vec3);
	}

	void VecTest_ScalarMultOperator()
	{
		LibVector<double> vec(1, 2, 3);
		auto result = vec * 2;
		MY_ASSERT_VEC_EQ(result, LibVector<double>(2, 4, 6));

		result = -1.0 * vec;
		MY_ASSERT_VEC_EQ(result, LibVector<double>(-1, -2, -3));

		result = vec * 0;
		MY_ASSERT_VEC_EQ(result, LibVector<double>(0, 0, 0));

		result = vec * 1;
		MY_ASSERT_VEC_EQ(result, LibVector<double>(1, 2, 3));

		result = 1e-10 * vec;
		MY_ASSERT_TRUE(result.IsZero());

		result = vec * 1e10;
		MY_ASSERT_VEC_EQ(result, LibVector<double>(1 * 1e10, 2 * 1e10, 3 * 1e10));

		result = 0.5 * vec;
		MY_ASSERT_VEC_EQ(result, LibVector<double>(0.5, 1, 1.5));
	}

	void VecTest_ScalarDivOperator()
	{
		LibVector<double> vec(1, 2, 3);
		auto result = vec / 2;
		MY_ASSERT_VEC_EQ(result, LibVector<double>(0.5, 1, 1.5));

		result = vec / -1;
		MY_ASSERT_VEC_EQ(result, LibVector<double>(-1, -2, -3));

		//result = vec / 0;

		result = vec / 1;
		MY_ASSERT_VEC_EQ(result, LibVector<double>(1, 2, 3));

		result = vec / 1e10;
		MY_ASSERT_TRUE(result.IsZero());

		result = vec / 1e-10;
		MY_ASSERT_VEC_EQ(result, LibVector<double>(1 * 1e10, 2 * 1e10, 3 * 1e10));

		result = vec / 0.5;
		MY_ASSERT_VEC_EQ(result, LibVector<double>(2, 4, 6));

		result = vec.CrossProduct(LibVector<double>(0, 0, 0));
		MY_ASSERT_TRUE(result.IsZero());
	}

	void LineTest_Basic() {
		Line line1(Pt(0, 0, 0), Vec(1, 0, 0));
		Line line2(Pt(0, 0, 0), Vec(1, 1, 0));
		MY_ASSERT_TRUE(LibEps::IsZero(line1.AngleBetweenLines(line2) - M_PI / 4));
		MY_ASSERT_FALSE(line1.IsParallel(line2));

		line2.SetDirection(Vec(3, 0, 0));
		MY_ASSERT_TRUE(LibEps::IsZero(line1.AngleBetweenLines(line2)));
		MY_ASSERT_TRUE(line1.IsParallel(line2));

		line2.SetDirection(Vec(-2, 0, 0));
		MY_ASSERT_TRUE(LibEps::IsZero(line1.AngleBetweenLines(line2) - M_PI));
		MY_ASSERT_TRUE(line1.IsOpposite(line2));

		line2.SetDirection(Vec(0, 1, 0));
		MY_ASSERT_TRUE(LibEps::IsZero(line1.AngleBetweenLines(line2) - M_PI / 2));
		MY_ASSERT_TRUE(line1.IsOrtogonal(line2));

		line2 = Line(Pt(0, 1, 0), Vec(0, 0, 1));
		MY_ASSERT_FALSE(line1.IsSkew(line2));

	}

	void MatrixTest_Equal()
	{
		LibMatrix<double> mtrx1 = LibMatrix<double>::IdentityMatrix();
		LibMatrix<double> mtrx2 = LibMatrix<double>::IdentityMatrix();

		MY_ASSERT_MTRX_EQ(mtrx1, mtrx2);

		mtrx1.SetNumb(1 + 1e-10, 0, 0);
		MY_ASSERT_FALSE(mtrx1.IsEqual(mtrx2, 1e-12));
		MY_ASSERT_TRUE(mtrx1.IsEqual(mtrx2));

		mtrx1.SetNumb(1 + 5e-9, 0, 0);
		MY_ASSERT_FALSE(mtrx1 == mtrx2);

		MY_ASSERT_TRUE(!(mtrx1 == LibMatrix<double>::ZeroMatrix()));
	}

	void MatrixTest_Translation()
	{
		LibMatrix<double> tr = LibMatrix<double>::TranslationInit(LibVector<double>(1, 2, 3));
		LibPoint<double> pt(2, 1, 3);

		MY_ASSERT_VEC_EQ(LibPoint<double>(3, 3, 6), LibMatrix<double>::MultPt(pt, tr));

		tr = LibMatrix<double>::TranslationInit(LibVector<double>(0, 0, 0));
		MY_ASSERT_VEC_EQ(pt, LibMatrix<double>::MultPt(pt, tr));

		tr = LibMatrix<double>::TranslationInit(LibVector<double>(-1, -1, -2));
		MY_ASSERT_VEC_EQ(LibPoint<double>(1, 0, 1), LibMatrix<double>::MultPt(pt, tr));
	}

	void MatrixTest_Scaling()
	{
		LibMatrix<double> sc = LibMatrix<double>::ScalingInit(LibVector<double>(1, 2, 3));
		LibPoint<double> pt(2, 1, 3);

		MY_ASSERT_VEC_EQ(LibPoint<double>(2, 2, 9), LibMatrix<double>::MultPt(pt, sc));

		sc = LibMatrix<double>::ScalingInit(LibVector<double>(1, 1, 1));
		MY_ASSERT_VEC_EQ(pt, LibMatrix<double>::MultPt(pt, sc));

		sc = LibMatrix<double>::ScalingInit(LibVector<double>(-1, -1, -1));
		MY_ASSERT_VEC_EQ(LibPoint<double>(-2, -1, -3), LibMatrix<double>::MultPt(pt, sc));

		sc = LibMatrix<double>::ScalingInit(LibVector<double>(-1, 2, 0));
		MY_ASSERT_VEC_EQ(LibPoint<double>(-2, 2, 0), LibMatrix<double>::MultPt(pt, sc));

		sc = LibMatrix<double>::ScalingInit(LibVector<double>(0, 0, 0));
		MY_ASSERT_VEC_EQ(LibPoint<double>(0, 0, 0), LibMatrix<double>::MultPt(pt, sc));
	}

	void MatrixTest_Rotation()
	{
		LibMatrix<double> mtrx = LibMatrix<double>::IdentityMatrix();
		MY_ASSERT_MTRX_EQ(mtrx, LibMatrix<double>::RotationX(0));
		MY_ASSERT_MTRX_EQ(mtrx, LibMatrix<double>::RotationX(2 * M_PI));
		MY_ASSERT_MTRX_EQ(mtrx, LibMatrix<double>::RotationY(0));
		MY_ASSERT_MTRX_EQ(mtrx, LibMatrix<double>::RotationY(2 * M_PI));
		MY_ASSERT_MTRX_EQ(mtrx, LibMatrix<double>::RotationZ(0));
		MY_ASSERT_MTRX_EQ(mtrx, LibMatrix<double>::RotationZ(2 * M_PI));

		mtrx.SetNumb(0, 1, 1); mtrx.SetNumb(1, 1, 2); mtrx.SetNumb(-1, 2, 1); mtrx.SetNumb(0, 2, 2);
		MY_ASSERT_MTRX_EQ(mtrx, LibMatrix<double>::RotationX(M_PI / 2));

		mtrx.SetNumb(-1, 1, 2); mtrx.SetNumb(1, 2, 1);;
		MY_ASSERT_MTRX_EQ(mtrx, LibMatrix<double>::RotationX(3 * M_PI / 2));

		mtrx = LibMatrix<double>::IdentityMatrix();
		mtrx.SetNumb(-1, 1, 1); mtrx.SetNumb(-1, 2, 2);
		MY_ASSERT_MTRX_EQ(mtrx, LibMatrix<double>::RotationX(M_PI));

		mtrx = LibMatrix<double>::ZeroMatrix();
		mtrx.SetNumb(-1, 0, 2); mtrx.SetNumb(1, 1, 1); mtrx.SetNumb(1, 2, 0); mtrx.SetNumb(1, 3, 3);
		MY_ASSERT_MTRX_EQ(mtrx, LibMatrix<double>::RotationY(M_PI / 2));
		mtrx.SetNumb(1, 0, 2); mtrx.SetNumb(-1, 2, 0);
		MY_ASSERT_MTRX_EQ(mtrx, LibMatrix<double>::RotationY(3 * M_PI / 2));

		mtrx = LibMatrix<double>::IdentityMatrix();
		mtrx.SetNumb(-1, 0, 0); mtrx.SetNumb(-1, 2, 2);
		MY_ASSERT_MTRX_EQ(mtrx, LibMatrix<double>::RotationY(M_PI));

		mtrx = LibMatrix<double>::ZeroMatrix();
		mtrx.SetNumb(1, 0, 1); mtrx.SetNumb(1, 3, 3);  mtrx.SetNumb(1, 2, 2); mtrx.SetNumb(-1, 1, 0);
		MY_ASSERT_MTRX_EQ(mtrx, LibMatrix<double>::RotationZ(M_PI / 2));
		mtrx.SetNumb(-1, 0, 1); mtrx.SetNumb(1, 1, 0);
		MY_ASSERT_MTRX_EQ(mtrx, LibMatrix<double>::RotationZ(3 * M_PI / 2));

		mtrx = LibMatrix<double>::IdentityMatrix();
		mtrx.SetNumb(-1, 0, 0); mtrx.SetNumb(-1, 1, 1);
		MY_ASSERT_MTRX_EQ(mtrx, LibMatrix<double>::RotationZ(M_PI));

		MY_ASSERT_TRUE(LibMatrix<double>::Rotation(LibVector<double>(1, 0, 0), M_PI / 34) == 
			LibMatrix<double>::RotationX(M_PI / 34));

		mtrx = LibMatrix<double>::IdentityMatrix();
		mtrx.SetNumb(0.5, 0, 0); mtrx.SetNumb(0.5, 0, 1); mtrx.SetNumb(- 1 / std::sqrt(2), 0, 2);
		mtrx.SetNumb(0.5, 1, 0); mtrx.SetNumb(0.5, 1, 1); mtrx.SetNumb(1 / std::sqrt(2), 1, 2);
		mtrx.SetNumb(1 / std::sqrt(2), 2, 0); mtrx.SetNumb(-1 / std::sqrt(2), 2, 1); mtrx.SetNumb(0, 2, 2);
		MY_ASSERT_TRUE(LibMatrix<double>::Rotation(LibVector<double>(1, 1, 0), M_PI / 2) == mtrx)
	}

	void MatrixTest_OpCombination() {
		LibMatrix<double> tr = LibMatrix<double>::TranslationInit(LibVector<double>(1, 2, 3));
		LibMatrix<double> sc = LibMatrix<double>::ScalingInit(LibVector<double>(2, -1, 0));

		LibPoint<double> pt(1, 2, 1);
		MY_ASSERT_VEC_EQ(LibMatrix<double>::MultPt(LibMatrix<double>::MultPt(pt, tr), sc), LibMatrix<double>::MultPt(pt, tr * sc));
		MY_ASSERT_VEC_EQ(LibMatrix<double>::MultPt(LibMatrix<double>::MultPt(pt, sc), tr), LibMatrix<double>::MultPt(pt, sc * tr));

		sc = LibMatrix<double>::ScalingInit(LibVector<double>(0, 0, 0));
		MY_ASSERT_TRUE(LibMatrix<double>::MultPt(LibMatrix<double>::MultPt(pt, sc), tr) == LibMatrix<double>::MultPt(pt, sc * tr));
		MY_ASSERT_TRUE(LibMatrix<double>::MultPt(LibMatrix<double>::MultPt(pt, tr), sc) == LibMatrix<double>::MultPt(pt, tr * sc));

		LibMatrix<double> rotX = LibMatrix<double>::RotationX(M_PI / 2);
		MY_ASSERT_VEC_EQ(LibMatrix<double>::MultPt(LibMatrix<double>::MultPt(pt, tr), rotX), LibMatrix<double>::MultPt(pt, tr * rotX));
		sc = LibMatrix<double>::ScalingInit(LibVector<double>(1, 0, 2));
		MY_ASSERT_VEC_EQ(LibMatrix<double>::MultPt(LibMatrix<double>::MultPt(
			LibMatrix<double>::MultPt(pt, tr), rotX), sc), LibMatrix<double>::MultPt(pt, tr * rotX * sc));

		rotX = LibMatrix<double>::RotationX(2 * M_PI);
		MY_ASSERT_TRUE(LibMatrix<double>::MultPt(pt, tr * rotX * sc) == LibMatrix<double>::MultPt(pt, tr * sc));
	}

	void MatrixTest_Inverse()
	{
		LibMatrix<double> mtrx1 = LibMatrix<double>::IdentityMatrix();
		auto inverse = mtrx1.InverseCopy();
		MY_ASSERT_MTRX_EQ(mtrx1, inverse);

		mtrx1.SetNumb(2, 0, 0); mtrx1.SetNumb(3, 1, 1); mtrx1.SetNumb(4, 2, 2); mtrx1.SetNumb(5, 3, 3);
		inverse.SetNumb(0.5, 0, 0); inverse.SetNumb(0.333333333, 1, 1);
		inverse.SetNumb(0.25, 2, 2); inverse.SetNumb(0.2, 3, 3);
		MY_ASSERT_MTRX_EQ(mtrx1.InverseCopy(), inverse);

		MY_ASSERT_TRUE(mtrx1 == mtrx1.InverseCopy().InverseCopy());
		MY_ASSERT_TRUE(mtrx1 * mtrx1.InverseCopy() == Mtrx::IdentityMatrix());
	}

	void CylTest_IntersLine()
	{
		LibCylinder<double> cyl(LibPoint<double>(0, 0, 0), LibVector<double>(0, 0, 1), 1);
		LibLine<double> line(LibPoint<double>(0, 0, 0), LibVector<double>(1, 1, 1));

		std::vector<Pt> res = cyl.IsIntersectionLine(line);
		MY_ASSERT_TRUE(res.size() == 2);
		MY_ASSERT_TRUE(res[0].IsEqual(Pt(0.71, 0.71, 0.71), 1e-2));
		MY_ASSERT_TRUE(res[1].IsEqual(Pt(-0.71, -0.71, -0.71), 1e-2));

		line.SetOrigin(Pt(1, 0, 0));
		line.SetDirection(Vec(0, 1, 0));
		res = cyl.IsIntersectionLine(line);
		MY_ASSERT_TRUE(res.size() == 1);
		MY_ASSERT_VEC_EQ(res[0], Pt(1, 0, 0));

		line.SetOrigin(Pt(2, 2, 0));
		line.SetDirection(Vec(1, 0, 1));
		res = cyl.IsIntersectionLine(line);
		
		MY_ASSERT_TRUE(res.empty());
	}

	void TriangleTest_IsPointOnTrngl()
	{
		Pt p1(2, 0, 0);
		Pt p2(2, 5, 0);
		Pt p3(0, 0, 0);
		Trngl trngl(p1, p2, p3);

		Pt onTrngl(1, 0, 0);
		MY_ASSERT_TRUE(trngl.IsPointOnTrngl(onTrngl));
		MY_ASSERT_TRUE(trngl.IsPointOnTrngl(p1));
		MY_ASSERT_TRUE(trngl.IsPointOnTrngl(p2));
		MY_ASSERT_TRUE(trngl.IsPointOnTrngl(p3));

		onTrngl = Pt(1.26, 0.88, 0);
		MY_ASSERT_TRUE(trngl.IsPointOnTrngl(onTrngl));

		onTrngl = Pt(2.7, 1.49, 0);
		MY_ASSERT_FALSE(trngl.IsPointOnTrngl(onTrngl));
		onTrngl = Pt(0, 0, 2);
		MY_ASSERT_FALSE(trngl.IsPointOnTrngl(onTrngl));
	}

	void TriangleTest_IntersLine()
	{
		Pt p1(2, 0, 0);
		Pt p2(2, 5, 0);
		Pt p3(0, 0, 0);
		Trngl trngl(p1, p2, p3);

		Line line(Pt(0, 0, 1), Vec(1.26, 0.88, -1));
		Pt inters;
		MY_ASSERT_TRUE(trngl.IsIntersectionLine(line, inters));
		MY_ASSERT_VEC_EQ(inters, Pt(1.26, 0.88, 0));

		line = Line(Pt(0, 0, 1), Vec(0.6, 0.2, 0));
		MY_ASSERT_FALSE(trngl.IsIntersectionLine(line, inters));
	}

	void ModelTest_CreateCube()
	{
		Pt center(0.5, 0.5, 0.5);
		Model cube(Model::CreateCube(center, 1.0));

		std::vector<Pt> points = cube.Points();
		Pt A(1, 0, 0); Pt B(1, 0, 1); Pt C(1, 1, 1); Pt D(1, 1, 0);
		Pt G(0, 0, 0); Pt E(0, 0, 1); Pt F(0, 1, 1); Pt K(0, 1, 0);
		std::vector<Pt> expPt = { A, D, C, B, D, K, F, C, K, G, E, F, G, A, B, E, B, C, F, E, G, K, D, A };
		MY_ASSERT_EQ(expPt, points);

		std::vector<Vec> normals = cube.Normals();
		Vec nrml1(1, 0, 0); Vec nrml2(0, 1, 0); Vec nrml3(-1, 0, 0);
		Vec nrml4(0, -1, 0); Vec nrml5(0, 0, 1); Vec nrml6(0, 0, -1);
		std::vector<Vec> expNrml = { nrml1, nrml1, nrml1, nrml1, nrml2, nrml2, nrml2, nrml2,
						nrml3, nrml3, nrml3, nrml3, nrml4, nrml4, nrml4, nrml4,
						nrml5, nrml5, nrml5, nrml5, nrml6, nrml6, nrml6, nrml6 };
		MY_ASSERT_EQ(expNrml, normals);

		std::vector<Srfc> surfaces = cube.Surfaces();
		std::vector<Srfc> expSrfc = { Srfc(0, 2), Srfc(2, 4), Srfc(4, 6),
			Srfc(6, 8), Srfc(8, 10), Srfc(10, 12) };
		MY_ASSERT_EQ(expSrfc, surfaces);

		std::vector<size_t> triangles = cube.Triangles();
		std::vector<size_t> expTr = { 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23};
		MY_ASSERT_EQ(expTr, triangles);
	}

	void ModelTest_CreateCylinder() {
		Pt center(0, 0, 0); Vec direction(0, 0, 1);
		Model cylinder = Model::CreateCylinder(center, direction, 1, 2, 0.5);

		MY_ASSERT_EQ(cylinder.Points().size(), 20);
		MY_ASSERT_EQ(cylinder.Normals().size(), 20);
		MY_ASSERT_EQ(cylinder.Triangles().size(), 36);
		MY_ASSERT_EQ(cylinder.Surfaces().size(), 3);

		std::vector<size_t> exp = { 0, 1, 2, 0, 2, 3, 0, 3, 1,
			4, 5, 6, 4, 6, 7, 4, 7, 5,
			1, 2, 6, 1, 6, 5, 2, 3, 7, 2, 7, 6, 3, 1, 5, 3, 5, 7 };
		MY_ASSERT_EQ(exp, cylinder.Triangles());
	}

	void ModelTest_CubeIntersRay() {
		Pt center(0.5, 0.5, 0.5);
		Model cube = Model::CreateCube(center, 1.0);

		std::ofstream out("data.bin", std::fstream::binary);
		cube.Save(out);
		out.close();

		Ray ray(Pt(1.22, 2, 1.5), Vec(-1.72, -2, -1.5));
		Pt pt; Srfc srfc;

		MY_ASSERT_TRUE(cube.IsIntersectionRay(ray, pt, srfc));
		MY_ASSERT_VEC_EQ(Pt(0.36, 1, 0.75), pt);
		MY_ASSERT_EQ(Srfc(2, 4), srfc);

		std::ifstream in("data.bin", std::fstream::binary);
		Model cube2;
		cube2.Load(in);
		in.close();

		Pt pt2; Srfc srfc2;

		MY_ASSERT_TRUE(cube.Points() == cube2.Points());
		MY_ASSERT_TRUE(cube.Normals() == cube2.Normals());
		MY_ASSERT_TRUE(cube.Triangles() == cube2.Triangles());
		MY_ASSERT_TRUE(cube.Surfaces() == cube2.Surfaces());

		MY_ASSERT_TRUE(cube2.IsIntersectionRay(ray, pt2, srfc2));
		MY_ASSERT_VEC_EQ(pt2, pt);
		MY_ASSERT_EQ(srfc2, srfc);

		ray.SetOrigin(Pt(0, 3, 1.5));
		ray.SetDirection(Vec(-0.5, -3, -1.5));
		MY_ASSERT_FALSE(cube.IsIntersectionRay(ray, pt, srfc));
	}

	void ModelTest_CylinderIntersRay() {
		Pt center(0, 0, 0); Vec direction(0, 0, 1);
		Model cylinder = Model::CreateCylinder(center, direction, 1, 2, 0.5);

		Ray ray(Pt(1.5, -1.5, 2), Vec(1.5, 3.5, 2));
		Pt pt; Srfc srfc;

		MY_ASSERT_FALSE(cylinder.IsIntersectionRay(ray, pt, srfc));

		ray.SetDirection(Vec(-3, 3, -1));
		MY_ASSERT_TRUE(cylinder.IsIntersectionRay(ray, pt, srfc));
		MY_ASSERT_EQ(Srfc(6, cylinder.Triangles().size() / 3), srfc);
		MY_ASSERT_EQ(Pt(0.5, -0.5, 1.666666666), pt);

		ray = Ray(Pt(1, -1, 1.83), Vec(0.5, -0.5, 0.17));
		MY_ASSERT_FALSE(cylinder.IsIntersectionRay(ray, pt, srfc));

		ray = Ray(Pt(0, 0, 1), Vec(1.5, -1.5, 1));
		MY_ASSERT_TRUE(cylinder.IsIntersectionRay(ray, pt, srfc));
		MY_ASSERT_EQ(Srfc(6, cylinder.Triangles().size() / 3), srfc);
		MY_ASSERT_EQ(Pt(0.5, -0.5, 1.333333333), pt);

		ray = Ray(Pt(1.5, -1.5, 2.7), Vec(-3, 3, -1.7));
		MY_ASSERT_TRUE(cylinder.IsIntersectionRay(ray, pt, srfc));
		MY_ASSERT_EQ(Srfc(3, 6), srfc);
		MY_ASSERT_EQ(Pt(0.264705882, -0.264705882, 2), pt);

		ray = Ray(Pt(0, 0, 3), Vec(0, 0, -1));
		MY_ASSERT_TRUE(cylinder.IsIntersectionRay(ray, pt, srfc));
		MY_ASSERT_EQ(Srfc(3, 6), srfc);
		MY_ASSERT_EQ(Pt(0, 0, 2), pt);

		ray = Ray(Pt(0, 0, -3), Vec(0, 0, 1));
		MY_ASSERT_TRUE(cylinder.IsIntersectionRay(ray, pt, srfc));
		MY_ASSERT_EQ(Srfc(0, 3), srfc);
		MY_ASSERT_EQ(Pt(0, 0, 0), pt);

		ray = Ray(Pt(1.5, -1.5, 0), Vec(0, 0, -1));
		MY_ASSERT_FALSE(cylinder.IsIntersectionRay(ray, pt, srfc));
	}

	void ModelTest_BigCylinder() {
		Pt center(0, 0, 0); Vec direction(0, 0, 1);

		TIMER_START("Create Cylinder without thread");
		Model cylinder = Model::CreateCylinder(center, direction, 1, 2, 1e-12);
		TIMER_END("Create Cylinder without thread");

		Ray ray = Ray(Pt(1.5, -1.5, 2.7), Vec(-3, 3, -1.7));

		Pt pt; Srfc srfc;
		TIMER_START("Big Cylinder without thread");
		MY_ASSERT_TRUE(cylinder.IsIntersectionRay(ray, pt, srfc));
		TIMER_END("Big Cylinder without thread");

		Pt pt2; Srfc srfc2;
		TIMER_START("Big Cylinder with thread");
		MY_ASSERT_TRUE(cylinder.IsIntersectionRayThread(ray, pt2, srfc2));
		TIMER_END("Big Cylinder with thread");

		MY_ASSERT_EQ(pt, pt2);
		MY_ASSERT_EQ(srfc, srfc2);

		TP tp(std::thread::hardware_concurrency());
		Pt pt3; Srfc srfc3;
		TIMER_START("Big Cylinder with threadPool");
		MY_ASSERT_TRUE(cylinder.IsIntersectionRayTP(ray, pt3, srfc3, tp));
		TIMER_END("Big Cylinder with threadPool");

		MY_ASSERT_EQ(pt, pt3);
		MY_ASSERT_EQ(srfc, srfc3);

	}

public:
	void RunAllTests() {
		RUN_TEST(PointTest_DistanceTo);
		RUN_TEST(PointTest_Operators);

		RUN_TEST(VecTest_Parallel);
		RUN_TEST(VecTest_IsZero);
		RUN_TEST(VecTest_EqOperators);
		RUN_TEST(VecTest_ScalarMultOperator);
		RUN_TEST(VecTest_ScalarDivOperator);

		RUN_TEST(LineTest_Basic);

		RUN_TEST(MatrixTest_Equal);
		RUN_TEST(MatrixTest_Translation);
		RUN_TEST(MatrixTest_Scaling);
		RUN_TEST(MatrixTest_Rotation);
		RUN_TEST(MatrixTest_OpCombination);
		RUN_TEST(MatrixTest_Inverse);

		RUN_TEST(CylTest_IntersLine);
		
		RUN_TEST(TriangleTest_IsPointOnTrngl);
		RUN_TEST(TriangleTest_IntersLine);

		RUN_TEST(ModelTest_CreateCube);
		RUN_TEST(ModelTest_CreateCylinder);
		RUN_TEST(ModelTest_CubeIntersRay);
		RUN_TEST(ModelTest_CylinderIntersRay);
		RUN_TEST(ModelTest_BigCylinder);
	}

	void SaveCube() {
		Pt center(0.5, 0.5, 0.5);
		Model cube = Model::CreateCube(center, 1.0);

		std::ofstream out("data.bin", std::fstream::binary);
		cube.Save(out);
		out.close();
	}
};

int main()
{
	Tests test;
	test.RunAllTests();
	test.SaveCube();
	TIMER_PRINT();
	return 0;
}
