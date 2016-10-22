/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ZTRACE_TESTING

#include "Random.h"
#include "Types.h"

using ztrace::Int;
using ztrace::Real;

#include "Gloss.h"
#include <iostream>
#include <gtest/gtest.h>

using Ray = ztrace::Ray;
using Vector = ztrace::Vector;
using TraceData = ztrace::TraceData;
using Gloss = ztrace::Gloss;

static Real const tol = 0.001;

TEST( Gloss_test, constructor_simplified )
{
    Gloss test{ true, false, false }; // should be a perfect black

    EXPECT_TRUE( test.hasDiffusive() );
    EXPECT_FALSE( test.hasSpecular() );
    EXPECT_FALSE( test.hasTransmission() );

    EXPECT_NEAR( test.fuzz(), 1.0, tol );
    EXPECT_NEAR( test.albedo().len(), 0.0, tol );
    EXPECT_NEAR( test.transmissivity().len(), 0.0, tol );
    EXPECT_NEAR( test.reflectivity().len(), 0.0, tol );
}

TEST( Material_test, dafaultBlack )
{
    Gloss const & perfectBlack = ztrace::perfectBlack;
    EXPECT_TRUE( perfectBlack.hasDiffusive() );
    EXPECT_FALSE( perfectBlack.hasSpecular() );
    EXPECT_FALSE( perfectBlack.hasTransmission() );

    EXPECT_NEAR( perfectBlack.fuzz(), 1.0, tol );
    EXPECT_NEAR( perfectBlack.albedo().len(), 0.0, tol );
    EXPECT_NEAR( perfectBlack.transmissivity().len(), 0.0, tol );
    EXPECT_NEAR( perfectBlack.reflectivity().len(), 0.0, tol );
}


GTEST_API_ int main( int argc, char ** argv ) {
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
