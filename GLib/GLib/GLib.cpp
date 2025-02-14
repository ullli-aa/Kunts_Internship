#include <iostream>

#include "MyTestMacros.h"
#include "LibPoint.h"
#include "LibVector.h"
#include "LibMatrix.h"
#include "LibCylinder.h"

class Tests {
public:
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


		LibCylinder<double> cyl(LibPoint<double>(0, 0, 0), LibVector<double>(0, 0, 1), 1);
		LibLine<double> line(LibPoint<double>(0, 0, 0), LibVector<double>(1, 1, 1));

		std::vector<LibPoint<double>> res = cyl.IsIntersectionLine(line);
	}

	void RunAllTests() {
		RUN_TEST(PointTest_DistanceTo);
		RUN_TEST(PointTest_Operators);
		RUN_TEST(VecTest_Parallel);
		RUN_TEST(VecTest_IsZero);
		RUN_TEST(VecTest_EqOperators);
		RUN_TEST(VecTest_ScalarMultOperator);
		RUN_TEST(VecTest_ScalarDivOperator);
		RUN_TEST(MatrixTest_Equal);
		RUN_TEST(MatrixTest_Translation);
		RUN_TEST(MatrixTest_Scaling);
		RUN_TEST(MatrixTest_Rotation);
		RUN_TEST(MatrixTest_OpCombination);
	}
};

int main()
{
	Tests test;
	test.RunAllTests();
}
