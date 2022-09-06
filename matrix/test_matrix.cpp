#include <gtest/gtest.h>
#include <cstddef>
#include <stdint.h>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "matrix.hpp"


class TestMatrix : public ::testing::Test
{
public:
	size_t rows_count = 3;
	size_t columns_count = 4;

	matrix::Matrix mat_0;
	/*
	 * 0 0 0 0
	 * 0 0 0 0
	 * 0 0 0 0
	 */
	matrix::Matrix mat_1;
	/*
	 * 1 1 1 1
	 * 1 1 1 1
	 * 1 1 1 1
	 */
	matrix::Matrix mat_2;
	/*
	 * 2 2 2 2
	 * 2 2 2 2
	 * 2 2 2 2
	 */
	matrix::Matrix mat_3;
	/*
	 * 3 3 3 3
	 * 3 3 3 3
	 * 3 3 3 3
	 */
	matrix::Matrix mat_ord1;
	/*
	 * 0 1 2  3
	 * 4 5 6  7
	 * 8 9 10 11
	 */
	matrix::Matrix mat_ord2;
	/*
	 * 0  2  4  6
	 * 8  10 12 14
	 * 16 18 20 22
	 */
	matrix::Matrix mat_ord1_1;
	/*
	 * 1 2  3  4
	 * 5 6  7  8
	 * 9 10 11 12
	 */
	matrix::Matrix mat_ord2_1;
	/*
	 * 1  3  5  7
	 * 9  11 13 15
	 * 17 19 21 23
	 */
	matrix::Matrix mat__1;
	/*
	 * -1 -1 -1 -1
	 * -1 -1 -1 -1
	 * -1 -1 -1 -1
	 */
	matrix::Matrix mat__2;
	/*
	 * -2 -2 -2 -2
	 * -2 -2 -2 -2
	 * -2 -2 -2 -2
	 */
	matrix::Matrix mat__3;
	/*
	 * -3 -3 -3 -3
	 * -3 -3 -3 -3
	 * -3 -3 -3 -3
	 */
	matrix::Matrix mat_ord_1;
	/*
	 *  0 -1 -2  -3
	 * -4 -5 -6  -7
	 * -8 -9 -10 -11
	 */
	matrix::Matrix mat_ord_2;
	/*
	 *  0  -2  -4  -6
	 * -8  -10 -12 -14
	 * -16 -18 -20 -22
	 */
	matrix::Matrix mat_ord_1_1;
	/*
	 * -1 -2  -3  -4
	 * -5 -6  -7  -8
	 * -9 -10 -11 -12
	 */
	matrix::Matrix mat_ord_2_1;
	/*
	 * -1  -3  -5  -7
	 * -9  -11 -13 -15
	 * -17 -19 -21 -23
	 */
	matrix::Matrix mat_1N;
	/*
	 * 0 1 2
	 */
	matrix::Matrix mat_N1;
	/*
	 * 0
	 * 1
	 * 2
	 */
	matrix::Matrix mat_11;
	/*
	 * 42
	 */

protected:
    virtual void SetUp()
	{
		mat_0 = matrix::Matrix(rows_count, columns_count, 0);

		mat_1 = matrix::Matrix(rows_count, columns_count, 1);
		mat_2 = matrix::Matrix(rows_count, columns_count, 2);
		mat_3 = matrix::Matrix(rows_count, columns_count, 3);
		mat_ord1 = matrix::Matrix(rows_count, columns_count);
		mat_ord2 = matrix::Matrix(rows_count, columns_count);
		mat_ord1_1 = matrix::Matrix(rows_count, columns_count);
		mat_ord2_1 = matrix::Matrix(rows_count, columns_count);

		mat__1 = matrix::Matrix(rows_count, columns_count, -1);
		mat__2 = matrix::Matrix(rows_count, columns_count, -2);
		mat__3 = matrix::Matrix(rows_count, columns_count, -3);
		mat_ord_1 = matrix::Matrix(rows_count, columns_count);
		mat_ord_2 = matrix::Matrix(rows_count, columns_count);
		mat_ord_1_1 = matrix::Matrix(rows_count, columns_count);
		mat_ord_2_1 = matrix::Matrix(rows_count, columns_count);

		mat_1N = matrix::Matrix(1, rows_count);
		mat_N1 = matrix::Matrix(rows_count, 1);
		mat_11 = matrix::Matrix(1, 1, 42);

		for (size_t i = 0; i < rows_count; i++)
		{
			for (size_t j = 0; j < columns_count; j++)
			{
				mat_ord1[i][j] = i * columns_count + j;
				mat_ord2[i][j] = 2 * (i * columns_count + j);
				mat_ord1_1[i][j] = i * columns_count + j + 1;
				mat_ord2_1[i][j] = 2 * (i * columns_count + j) + 1;

				mat_ord_1[i][j] = -(i * columns_count + j);
				mat_ord_2[i][j] = -2 * (i * columns_count + j);
				mat_ord_1_1[i][j] = -(i * columns_count + j + 1);
				mat_ord_2_1[i][j] = -2 * (i * columns_count + j) - 1;
			}
			mat_1N[0][i] = i;
			mat_N1[i][0] = i;
		}
	}

    virtual void TearDown() {}
};


TEST_F(TestMatrix, test_matrix_constructors_and_getters)
{
	matrix::Matrix mt1(rows_count, columns_count);
	ASSERT_EQ(mt1.GetRowsCount(), rows_count);
	ASSERT_EQ(mt1.GetColumnsCount(), columns_count);
	ASSERT_EQ(mt1, mat_0);

	matrix::Matrix mt2 = mt1;
	ASSERT_EQ(mt2.GetRowsCount(), rows_count);
	ASSERT_EQ(mt2.GetColumnsCount(), columns_count);
	ASSERT_EQ(mt2, mt1);

	mt2[0][0] = 1;
	ASSERT_NE(mt2, mt1);

	matrix::Matrix mt3(rows_count, columns_count, 1);
	ASSERT_EQ(mt3.GetRowsCount(), rows_count);
	ASSERT_EQ(mt3.GetColumnsCount(), columns_count);
	ASSERT_EQ(mt3, mat_1);

	matrix::Matrix mt4(rows_count, columns_count, -1);
	ASSERT_EQ(mt4.GetRowsCount(), rows_count);
	ASSERT_EQ(mt4.GetColumnsCount(), columns_count);
	ASSERT_EQ(mt4, mat__1);

	matrix::Matrix mt_1N(1, rows_count);
	ASSERT_EQ(mt_1N.GetRowsCount(), 1);
	ASSERT_EQ(mt_1N.GetColumnsCount(), rows_count);

	matrix::Matrix mt_N1(rows_count, 1);
	ASSERT_EQ(mt_N1.GetRowsCount(), rows_count);
	ASSERT_EQ(mt_N1.GetColumnsCount(), 1);

	matrix::Matrix mt_11 (1, 1);
	ASSERT_EQ(mt_11.GetRowsCount(), 1);
	ASSERT_EQ(mt_11.GetColumnsCount(), 1);
}

TEST_F(TestMatrix, test_matrix_constructor_empty)
{
	matrix::Matrix mt1;
	ASSERT_EQ(mt1.GetRowsCount(), 0);
	ASSERT_EQ(mt1.GetColumnsCount(), 0);

	matrix::Matrix mt2(0, 0);
	ASSERT_EQ(mt2.GetRowsCount(), 0);
	ASSERT_EQ(mt2.GetColumnsCount(), 0);

	matrix::Matrix mt3(rows_count, 0);
	ASSERT_EQ(mt3.GetRowsCount(), 0);
	ASSERT_EQ(mt3.GetColumnsCount(), 0);

	matrix::Matrix mt4(0, columns_count);
	ASSERT_EQ(mt4.GetRowsCount(), 0);
	ASSERT_EQ(mt4.GetColumnsCount(), 0);

	matrix::Matrix mt5(rows_count, 1234567890123);
	ASSERT_EQ(mt5.GetRowsCount(), 0);
	ASSERT_EQ(mt5.GetColumnsCount(), 0);

	matrix::Matrix mt6(1234567890123, columns_count);
	ASSERT_EQ(mt6.GetRowsCount(), 0);
	ASSERT_EQ(mt6.GetColumnsCount(), 0);

	matrix::Matrix mt7(10000, 10000);
	ASSERT_EQ(mt7.GetRowsCount(), 10000);
	ASSERT_EQ(mt7.GetColumnsCount(), 10000);
}

TEST_F(TestMatrix, test_matrix_assign)
{
	matrix::Matrix mt1;

	ASSERT_NE(mt1, mat_1);
	mt1 = mat_1;
	ASSERT_EQ(mt1, mat_1);

	ASSERT_NE(mt1, mat_2);
	mt1 = mat_2;
	ASSERT_EQ(mt1, mat_2);

	ASSERT_NE(mt1, mat_3);
	mt1 = mat_3;
	ASSERT_EQ(mt1, mat_3);

	ASSERT_NE(mt1, mat__1);
	mt1 = mat__1;
	ASSERT_EQ(mt1, mat__1);

	ASSERT_NE(mt1, mat__2);
	mt1 = mat__2;
	ASSERT_EQ(mt1, mat__2);

	ASSERT_NE(mt1, mat__3);
	mt1 = mat__3;
	ASSERT_EQ(mt1, mat__3);
}

TEST_F(TestMatrix, test_matrix_elem_access)
{
	matrix::Matrix mt1 = mat_ord1;

	for (size_t i = 0; i < rows_count; i++)
		for (size_t j = 0; j < columns_count; j++)
			ASSERT_EQ(mt1[i][j], i * columns_count + j);


	for (size_t i = 0; i < rows_count; i++)
		for (size_t j = 0; j < columns_count; j++)
			mt1[i][j]++;

	for (size_t i = 0; i < rows_count; i++)
		for (size_t j = 0; j < columns_count; j++)
			ASSERT_EQ(mt1[i][j], i * columns_count + j + 1);

	ASSERT_THROW(mt1[rows_count][0], std::out_of_range);
	ASSERT_THROW(mt1[0][columns_count], std::out_of_range);
	ASSERT_THROW(mt1[rows_count][columns_count], std::out_of_range);


	mt1 = mat_ord_1;

	for (size_t i = 0; i < rows_count; i++)
		for (size_t j = 0; j < columns_count; j++)
			ASSERT_EQ(mt1[i][j], -(i * columns_count + j));


	for (size_t i = 0; i < rows_count; i++)
		for (size_t j = 0; j < columns_count; j++)
			mt1[i][j]--;

	for (size_t i = 0; i < rows_count; i++)
		for (size_t j = 0; j < columns_count; j++)
			ASSERT_EQ(mt1[i][j], -(i * columns_count + j) - 1);

	ASSERT_THROW(mt1[rows_count][0], std::out_of_range);
	ASSERT_THROW(mt1[0][columns_count], std::out_of_range);
	ASSERT_THROW(mt1[rows_count][columns_count], std::out_of_range);
}

TEST_F(TestMatrix, test_matrix_multiply)
{
	matrix::Matrix mt1 = mat_1;

	mt1 *= 1;
	ASSERT_EQ(mt1, mat_1);

	mt1 *= -1;
	ASSERT_EQ(mt1, mat__1);

	mt1 *= -3;
	ASSERT_EQ(mt1, mat_3);

	mt1 *= -1;
	ASSERT_EQ(mt1, mat__3);

	mt1 *= 0;
	ASSERT_EQ(mt1, mat_0);

	mt1 *= 3;
	ASSERT_EQ(mt1, mat_0);

	mt1 *= -3;
	ASSERT_EQ(mt1, mat_0);

	matrix::Matrix mt2 = mat_ord1;

	mt2 *= -2;
	ASSERT_EQ(mt2, mat_ord_2);

	mt2 *= -1;
	ASSERT_EQ(mt2, mat_ord2);
}

TEST_F(TestMatrix, test_matrix_plus)
{
	matrix::Matrix mt0 = mat_0;

	matrix::Matrix mt1 = mat_1;
	matrix::Matrix mt2 = mat_2;
	matrix::Matrix mt3 = mat_3;
	matrix::Matrix mt_ord1 = mat_ord1;
	matrix::Matrix mt_ord1_1 = mat_ord1_1;

	matrix::Matrix mt_1 = mat__1;
	matrix::Matrix mt_2 = mat__2;
	matrix::Matrix mt_3 = mat__3;
	matrix::Matrix mt_ord_1 = mat_ord_1;
	matrix::Matrix mt_ord_1_1 = mat_ord_1_1;

	matrix::Matrix mt_1N = mt_1N;
	matrix::Matrix mt_N1 = mat_N1;
	matrix::Matrix mt_11 = mat_11;


	ASSERT_EQ(mt0 + mt1, mt1);
	ASSERT_EQ(mt0 + mt_1, mt_1);

	ASSERT_EQ(mt1 + mt_1, mt0);
	ASSERT_EQ(mt2 + mt_2, mt0);
	ASSERT_EQ(mt3 + mt_3, mt0);

	ASSERT_EQ(mt1 + mt1, mt2);
	ASSERT_EQ(mt1 + mt2, mt3);
	ASSERT_EQ(mt_1 + mt_1, mt_2);
	ASSERT_EQ(mt_1 + mt_2, mt_3);

	ASSERT_EQ(mt1 + mt_ord1, mt_ord1_1);
	ASSERT_EQ(mt_1 + mt_ord_1, mt_ord_1_1);

	ASSERT_EQ(mt1 + mt2, mt2 + mt1);
	ASSERT_EQ(mt_3 + mt_ord1, mt_ord1 + mt_3);

	ASSERT_EQ((mt1 + mt_ord_1) + mt3, mt1 + (mt_ord_1 + mt3));

	ASSERT_THROW(mt0 + mt_1N, std::length_error);
	ASSERT_THROW(mt0 + mt_N1, std::length_error);
	ASSERT_THROW(mt0 + mt_11, std::length_error);
	ASSERT_THROW(mt_1N + mt_N1, std::length_error);
	ASSERT_THROW(mt_N1 + mt_1N, std::length_error);

	ASSERT_EQ(mt0, mat_0);

	ASSERT_EQ(mt1, mat_1);
	ASSERT_EQ(mt2, mat_2);
	ASSERT_EQ(mt3, mat_3);
	ASSERT_EQ(mt_ord1, mat_ord1);
	ASSERT_EQ(mt_ord1_1, mat_ord1_1);

	ASSERT_EQ(mt_1, mat__1);
	ASSERT_EQ(mt_2, mat__2);
	ASSERT_EQ(mt_3, mat__3);
	ASSERT_EQ(mt_ord_1, mat_ord_1);
	ASSERT_EQ(mt_ord_1_1, mat_ord_1_1);
}

TEST_F(TestMatrix, test_matrix_equal)
{
	matrix::Matrix mt0 = mat_0;

	matrix::Matrix mt1 = mat_1;
	matrix::Matrix mt2 = mat_2;
	matrix::Matrix mt3 = mat_3;
	matrix::Matrix mt_ord1 = mat_ord1;
	matrix::Matrix mt_ord2 = mat_ord2;
	matrix::Matrix mt_ord1_1 = mat_ord1_1;
	matrix::Matrix mt_ord2_1 = mat_ord2_1;

	matrix::Matrix mt_1 = mat__1;
	matrix::Matrix mt_2 = mat__2;
	matrix::Matrix mt_3 = mat__3;
	matrix::Matrix mt_ord_1 = mat_ord_1;
	matrix::Matrix mt_ord_2 = mat_ord_2;
	matrix::Matrix mt_ord_1_1 = mat_ord_1_1;
	matrix::Matrix mt_ord_2_1 = mat_ord_2_1;

	ASSERT_TRUE(mt0 == mat_0);

	ASSERT_TRUE(mt1 == mat_1);
	ASSERT_TRUE(mt2 == mat_2);
	ASSERT_TRUE(mt3 == mat_3);
	ASSERT_TRUE(mt_ord1 == mat_ord1);
	ASSERT_TRUE(mt_ord2 == mat_ord2);
	ASSERT_TRUE(mt_ord1_1 == mat_ord1_1);
	ASSERT_TRUE(mt_ord2_1 == mat_ord2_1);

	ASSERT_TRUE(mt_1 == mat__1);
	ASSERT_TRUE(mt_2 == mat__2);
	ASSERT_TRUE(mt_3 == mat__3);
	ASSERT_TRUE(mt_ord_1 == mat_ord_1);
	ASSERT_TRUE(mt_ord_2 == mat_ord_2);
	ASSERT_TRUE(mt_ord_1_1 == mat_ord_1_1);
	ASSERT_TRUE(mt_ord_2_1 == mat_ord_2_1);

	ASSERT_FALSE(mt0 == mt1);
	ASSERT_FALSE(mt1 == mt_1);
	ASSERT_FALSE(mt2 == mt_2);
	ASSERT_FALSE(mt3 == mt_3);
	ASSERT_FALSE(mt_ord1 == mt_ord_1);
	ASSERT_FALSE(mt_ord2 == mt_ord_2);
	ASSERT_FALSE(mt_ord1_1 == mt_ord_1_1);
	ASSERT_FALSE(mt_ord2_1 == mt_ord_2_1);
}

TEST_F(TestMatrix, test_matrix_not_equal)
{
	matrix::Matrix mt0 = mat_0;

	matrix::Matrix mt1 = mat_1;
	matrix::Matrix mt2 = mat_2;
	matrix::Matrix mt3 = mat_3;
	matrix::Matrix mt_ord1 = mat_ord1;
	matrix::Matrix mt_ord2 = mat_ord2;
	matrix::Matrix mt_ord1_1 = mat_ord1_1;
	matrix::Matrix mt_ord2_1 = mat_ord2_1;

	matrix::Matrix mt_1 = mat__1;
	matrix::Matrix mt_2 = mat__2;
	matrix::Matrix mt_3 = mat__3;
	matrix::Matrix mt_ord_1 = mat_ord_1;
	matrix::Matrix mt_ord_2 = mat_ord_2;
	matrix::Matrix mt_ord_1_1 = mat_ord_1_1;
	matrix::Matrix mt_ord_2_1 = mat_ord_2_1;

	ASSERT_FALSE(mt0 != mat_0);

	ASSERT_FALSE(mt1 != mat_1);
	ASSERT_FALSE(mt2 != mat_2);
	ASSERT_FALSE(mt3 != mat_3);
	ASSERT_FALSE(mt_ord1 != mat_ord1);
	ASSERT_FALSE(mt_ord2 != mat_ord2);
	ASSERT_FALSE(mt_ord1_1 != mat_ord1_1);
	ASSERT_FALSE(mt_ord2_1 != mat_ord2_1);

	ASSERT_FALSE(mt_1 != mat__1);
	ASSERT_FALSE(mt_2 != mat__2);
	ASSERT_FALSE(mt_3 != mat__3);
	ASSERT_FALSE(mt_ord_1 != mat_ord_1);
	ASSERT_FALSE(mt_ord_2 != mat_ord_2);
	ASSERT_FALSE(mt_ord_1_1 != mat_ord_1_1);
	ASSERT_FALSE(mt_ord_2_1 != mat_ord_2_1);

	ASSERT_TRUE(mt0 != mt1);
	ASSERT_TRUE(mt1 != mt_1);
	ASSERT_TRUE(mt2 != mt_2);
	ASSERT_TRUE(mt3 != mt_3);
	ASSERT_TRUE(mt_ord1 != mt_ord_1);
	ASSERT_TRUE(mt_ord2 != mt_ord_2);
	ASSERT_TRUE(mt_ord1_1 != mt_ord_1_1);
	ASSERT_TRUE(mt_ord2_1 != mt_ord_2_1);
}

TEST_F(TestMatrix, test_matrix_big_int)
{
	matrix::Matrix mt31_1 = mat_1;
	mt31_1 *= 2147483647;

	matrix::Matrix mt_31_1 = mt31_1;
	mt_31_1 *= -1;

	matrix::Matrix mt_31 = mt_31_1 + mat__1;

	ASSERT_EQ(mt31_1 + mat_1, mt_31);
	ASSERT_EQ(mt_31 + mat__1, mt31_1);
}

TEST_F(TestMatrix, test_matrix_output)
{
	std::ostringstream oss;
	oss << mat_0;
	ASSERT_EQ(oss.str(), "0 0 0 0 \n0 0 0 0 \n0 0 0 0 \n");

	oss.str(std::string());
	oss << mat_ord_1;
	ASSERT_EQ(oss.str(), "0 -1 -2 -3 \n-4 -5 -6 -7 \n-8 -9 -10 -11 \n");

	oss.str(std::string());
	oss << mat_1N;
	ASSERT_EQ(oss.str(), "0 1 2 \n");

	oss.str(std::string());
	oss << mat_N1;
	ASSERT_EQ(oss.str(), "0 \n1 \n2 \n");

	oss.str(std::string());
	oss << mat_11;
	ASSERT_EQ(oss.str(), "42 \n");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}