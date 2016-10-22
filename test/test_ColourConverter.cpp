/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ColourConverter.h"
#include <iostream>
#include <gtest/gtest.h>

using ztrace::Int;
using ztrace::Real;

template<Int colourRange>
using ColourConverter = ztrace::ColourConverter<colourRange>;

TEST( Colour_test, constructor_real )
{
    ColourConverter<128> test{ };
    test.setRGB( (Real) 0.5, (Real) 1.5, (Real) -0.5 );
    EXPECT_EQ( test.red(), 64 );
    EXPECT_EQ( test.green(), 128 );
    EXPECT_EQ( test.blue(), 0 );
}

TEST( Colour_test, toStdout )
{
    ColourConverter<64> test{ };
    test.setRGB( (Real) 0.25, (Real) 0.5, (Real) 0.75 );
    testing::internal::CaptureStdout();
    std::cout << test;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ( output, "16 32 48" );
}

GTEST_API_ int main( int argc, char ** argv ) {
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
