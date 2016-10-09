//
// Created by Axel Zschutschke on 10/8/16.
//

#include "Ray.h"
#include <iostream>
#include <gtest/gtest.h>

using ztrace::Int;
using ztrace::Real;

using Ray = ztrace::Ray;
using Vector = ztrace::Vector;

TEST( Ray_test, constructor_dummy )
{
    Ray test{ };
    EXPECT_EQ( test.origin().len(), 0. );
    EXPECT_EQ( test.direction().len(), 0. );
    EXPECT_EQ( test.positionLength( 1.0 ).len(), 0. );
}

TEST( Ray_test, constructor_full )
{
    Vector a( 2., 0., 0. );
    Vector b( 1., 0., 0. );
    Ray test{ a, b };
    EXPECT_EQ( test.origin().len(), 2. );
    EXPECT_EQ( test.direction().len(), 1. );
    EXPECT_EQ( test.positionLength( 1.0 ).len(), 3. );
}

TEST( Ray_test, positionLength )
{
    Vector a( 2., 0., 0. );
    Vector b( 1., -1., 0. );
    Ray test{ a, b };
    Vector c = test.positionLength( 2. );
    EXPECT_EQ( c.x(),  4. );
    EXPECT_EQ( c.y(), -2. );
    EXPECT_EQ( c.z(),  0. );
}

GTEST_API_ int main( int argc, char ** argv ) {
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
