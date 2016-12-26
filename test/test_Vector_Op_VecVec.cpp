/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Vector.h"
#include <iostream>
#include <gtest/gtest.h>

using ztrace::Int;
using ztrace::Real;

using Vector = ztrace::Vector;

static Real const tol = (Real) 0.001;

TEST(Vector_test, operatorAddAssign)
{
    Vector a{1.0, 2.0, 3.0};
    Vector b{3.0, 2.0, 1.0};
    a += b;
    EXPECT_NEAR(a.x(), 4.0, tol);
    EXPECT_NEAR(a.y(), 4.0, tol);
    EXPECT_NEAR(a.z(), 4.0, tol);
}
TEST(Vector_test, operatorSubAssign)
{
    Vector a{1.0, 2.0, 3.0};
    Vector b{3.0, 2.0, 1.0};
    a -= b;
    EXPECT_NEAR(a.x(), -2.0, tol);
    EXPECT_NEAR(a.y(), 0.0, tol);
    EXPECT_NEAR(a.z(), 2.0, tol);
}
TEST(Vector_test, operatorMulAssign)
{
    Vector a{1.0, 2.0, 3.0};
    Vector b{3.0, 2.0, 1.0};
    a *= b;
    EXPECT_NEAR(a.x(), 3.0, tol);
    EXPECT_NEAR(a.y(), 4.0, tol);
    EXPECT_NEAR(a.z(), 3.0, tol);
}
TEST(Vector_test, operatorDivAssign)
{
    Vector a{1.0, 2.0, 3.0};
    Vector b{3.0, 2.0, 1.0};
    a /= b;
    EXPECT_NEAR(a.x(), 1.0 / 3.0, tol);
    EXPECT_NEAR(a.y(), 1.0, tol);
    EXPECT_NEAR(a.z(), 3.0, tol);
}
TEST(Vector_test, operatorAdd)
{
    Vector a{1.0, 2.0, 3.0};
    Vector b{3.0, 2.0, 1.0};
    Vector c = a + b;
    EXPECT_NEAR(c.x(), 4.0, tol);
    EXPECT_NEAR(c.y(), 4.0, tol);
    EXPECT_NEAR(c.z(), 4.0, tol);
}
TEST(Vector_test, operatorSub)
{
    Vector a{1.0, 2.0, 3.0};
    Vector b{3.0, 2.0, 1.0};
    Vector c = a - b;
    EXPECT_NEAR(c.x(), -2.0, tol);
    EXPECT_NEAR(c.y(), 0.0, tol);
    EXPECT_NEAR(c.z(), 2.0, tol);
}
TEST(Vector_test, operatorMul)
{
    Vector a{1.0, 2.0, 3.0};
    Vector b{3.0, 2.0, 1.0};
    Vector c = a * b;
    EXPECT_NEAR(c.x(), 3.0, tol);
    EXPECT_NEAR(c.y(), 4.0, tol);
    EXPECT_NEAR(c.z(), 3.0, tol);
}
TEST(Vector_test, operatorDiv)
{
    Vector a{1.0, 2.0, 3.0};
    Vector b{3.0, 2.0, 1.0};
    Vector c = a / b;
    EXPECT_NEAR(c.x(), 1.0 / 3.0, tol);
    EXPECT_NEAR(c.y(), 1.0, tol);
    EXPECT_NEAR(c.z(), 3.0, tol);
}

GTEST_API_ int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
