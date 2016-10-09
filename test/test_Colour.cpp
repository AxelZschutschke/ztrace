//
// Created by Axel Zschutschke on 10/8/16.
//

#include "Colour.h"
#include <iostream>
#include <gtest/gtest.h>

using ztrace::Int;
using ztrace::Real;

template<Int colourRange>
using Colour = ztrace::Colour<colourRange>;

TEST( Colour_test, constructor_real )
{
    Colour<128> test{ };
    test.setRGB( 0.5, 1.5, -0.5 );
    EXPECT_EQ( test.redValue().getInt(), 64 );
    EXPECT_EQ( test.greenValue().getInt(), 128 );
    EXPECT_EQ( test.blueValue().getInt(), 0 );
}

TEST( Colour_test, toStdout )
{
    Colour<64> test{ };
    test.setRGB( 0.25, 0.5, 0.75 );
    testing::internal::CaptureStdout();
    std::cout << test;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ( output, "16 32 48" );
}

GTEST_API_ int main( int argc, char ** argv ) {
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
