#include <iostream>

#include "MyTestMacros.h"
#include "LibPoint.h"
#include "LibVector.h"
#include "LibPlane.h"

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

	void RunAllTests() {
		RUN_TEST(PointTest_DistanceTo);
		RUN_TEST(PointTest_Operators);
		RUN_TEST(VecTest_Parallel);
		RUN_TEST(VecTest_IsZero);
		RUN_TEST(VecTest_EqOperators);
		RUN_TEST(VecTest_ScalarMultOperator);
		RUN_TEST(VecTest_ScalarDivOperator);
	}
};

int main()
{
	Tests test;
	test.RunAllTests();
}
