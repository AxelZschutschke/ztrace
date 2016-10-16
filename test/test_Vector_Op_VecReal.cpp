//
// Created by Axel Zschutschke on 10/8/16.
//

#include "Vector.h"
#include <iostream>
#include <gtest/gtest.h>

using ztrace::Int;
using ztrace::Real;

using Vector = ztrace::Vector;

static Real const tol = 0.001;

TEST( Vector_test, operatorAddAssign )
{
    Vector a{ 1.0, 2.0, 3.0 };
    a += 3.0;
    EXPECT_NEAR(  a.x(), 4.0 , tol );
    EXPECT_NEAR(  a.y(), 5.0 , tol );
    EXPECT_NEAR(  a.z(), 6.0 , tol );
}
TEST( Vector_test, operatorSubAssign )
{
    Vector a{ 1.0, 2.0, 3.0 };
    a -= 1.;
    EXPECT_NEAR(  a.x(),  0.0 , tol );
    EXPECT_NEAR(  a.y(),  1.0 , tol );
    EXPECT_NEAR(  a.z(),  2.0 , tol );
}
TEST( Vector_test, operatorMulAssign )
{
    Vector a{ 1.0, 2.0, 3.0 };
    a *= -2.0;
    EXPECT_NEAR(  a.x(), -2.0 , tol );
    EXPECT_NEAR(  a.y(), -4.0 , tol );
    EXPECT_NEAR(  a.z(), -6.0 , tol );
}
TEST( Vector_test, operatorDivAssign )
{
    Vector a{ 1.0, 2.0, 3.0 };
    a /= 0.5;
    EXPECT_NEAR(  a.x(),  2.0 , tol );
    EXPECT_NEAR(  a.y(),  4.0 , tol );
    EXPECT_NEAR(  a.z(),  6.0 , tol );
}
TEST( Vector_test, operatorAdd )
{
    Vector a{ 1.0, 2.0, 3.0 };
    Vector c = a + 1.5;
    EXPECT_NEAR(  c.x(), 2.5 , tol );
    EXPECT_NEAR(  c.y(), 3.5 , tol );
    EXPECT_NEAR(  c.z(), 4.5 , tol );
}
TEST( Vector_test, operatorSub )
{
    Vector a{ 1.0, 2.0, 3.0 };
    Vector c = a - 1.3;
    EXPECT_NEAR( c.x(), -0.3, tol );
    EXPECT_NEAR( c.y(),  0.7, tol );
    EXPECT_NEAR( c.z(),  1.7, tol );
}
TEST( Vector_test, operatorMul )
{
    Vector a{ 1.0, 2.0, 3.0 };
    Vector c = a * -2.;
    EXPECT_NEAR(  c.x(),  -2.0 , tol );
    EXPECT_NEAR(  c.y(),  -4.0 , tol );
    EXPECT_NEAR(  c.z(),  -6.0 , tol );
}
TEST( Vector_test, operatorDiv )
{
    Vector a{ 1.0, 2.0, 3.0 };
    Vector c = a / -0.1;
    EXPECT_NEAR(  c.x(), -10.0 , tol );
    EXPECT_NEAR(  c.y(), -20.0 , tol );
    EXPECT_NEAR(  c.z(), -30.0 , tol );
}

GTEST_API_ int main( int argc, char ** argv ) {
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
