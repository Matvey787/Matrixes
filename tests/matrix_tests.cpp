#include <gtest/gtest.h>
#include "matrix.hpp"

TEST(MatrixTest, DeterminantIdentity2x2)
{
    Matrix::Matrix<double> mat(2,{1, 0,
                                0, 1});
    EXPECT_DOUBLE_EQ(mat.getDet(), 1);
}

TEST(MatrixTest, DeterminantIdentity3x3)
{
    Matrix::Matrix<double> mat(3, {1, 0, 0,
                                0, 1, 0,
                                0, 0, 1});
    EXPECT_DOUBLE_EQ(mat.getDet(), 1);
}

TEST(MatrixTest, DeterminantZeroMatrix)
{
    Matrix::Matrix<double> mat(2, {0, 0,
                                0, 0});
    EXPECT_DOUBLE_EQ(mat.getDet(), 0);
}

TEST(MatrixTest, DeterminantSimple2x2)
{
    Matrix::Matrix<double> mat(2, {2, 1,
                        5, 3});
    EXPECT_DOUBLE_EQ(mat.getDet(), 1);
}

TEST(MatrixTest, DeterminantSimple3x3)
{
    Matrix::Matrix<double> mat(3, {6, 1, 1,
                                4, -2, 5,
                                2, 8, 7 });
    EXPECT_DOUBLE_EQ(mat.getDet(), -306);
}

TEST(MatrixTest, DeterminantSingularMatrix)
{
    Matrix::Matrix<double> mat(2, {2, 4,
                                1, 2});
    EXPECT_DOUBLE_EQ(mat.getDet(), 0);
}

TEST(MatrixTest, DeterminantUpperTriangular)
{
    Matrix::Matrix<double> mat(3, {3, 2, 1,
                                0, 4, 5,
                                0, 0, 6});
    EXPECT_DOUBLE_EQ(mat.getDet(), 3 * 4 * 6);
}

TEST(MatrixTest, DeterminantLowerTriangular)
{
    Matrix::Matrix<double> mat(3, {7, 0, 0,
                                2, 8, 0,
                                3, 4, 9});
    EXPECT_DOUBLE_EQ(mat.getDet(), 7 * 8 * 9);
}
