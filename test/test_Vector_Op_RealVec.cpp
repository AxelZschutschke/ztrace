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

TEST( Vector_test, operatorAdd )
{
    Vector a{ 1.0, 2.0, 3.0 };
    Vector c = 2.5 + a;
    EXPECT_NEAR(  c.x(), 3.5 , tol );
    EXPECT_NEAR(  c.y(), 4.5 , tol );
    EXPECT_NEAR(  c.z(), 5.5 , tol );
}
TEST( Vector_test, operatorSub )
{
    Vector a{ 1.0, 2.0, 3.0 };
    Vector c = 2. - a;
    EXPECT_NEAR(  c.x(),  1.0 , tol );
    EXPECT_NEAR(  c.y(),  0.0 , tol );
    EXPECT_NEAR(  c.z(), -1.0 , tol );
}
TEST( Vector_test, operatorMul )
{
    Vector a{ 1.0, -2.0, 3.0 };
    Vector c = 0.5 * a;
    EXPECT_NEAR(  c.x(),  0.5 , tol );
    EXPECT_NEAR(  c.y(), -1.0 , tol );
    EXPECT_NEAR(  c.z(),  1.5 , tol );
}
TEST( Vector_test, operatorDiv )
{
    Vector a{ 1.0, 2.0, -5.0 };
    Vector c = 5. / a;
    EXPECT_NEAR(  c.x(),  5.0, tol );
    EXPECT_NEAR(  c.y(),  2.5 , tol );
    EXPECT_NEAR(  c.z(), -1.0 , tol );
}

GTEST_API_ int main( int argc, char ** argv ) {
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
