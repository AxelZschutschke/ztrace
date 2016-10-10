//
// Created by Axel Zschutschke on 10/8/16.
//

#include "Image.h"
#include "ColourConverter.h"
#include <iostream>
#include <gtest/gtest.h>

using ztrace::Int;
using ztrace::Real;
using ztrace::Size;

using Vector = ztrace::Vector;

template<Int colourRange>
using Colour = ztrace::ColourConverter<colourRange>;

template<Int colourRange>
using Image = ztrace::Image<Vector,Colour<colourRange>>;

TEST( Image_test, constructor_real )
{
    Image<128> test(1, 1);
    Vector colour( 0.5, 0.5, 0.5 );
    test.setPixel( 0, 0, colour );
    EXPECT_EQ( test.size(), 1 );
    EXPECT_EQ( test.width(), 1 );
    EXPECT_EQ( test.height(), 1 );
}

TEST( Image_test, toStdout )
{
    Image<128> test(1, 1);
    Vector colour( 0.5, 0.5, 0.5 );
    test.setPixel( 0, 0, colour );
    testing::internal::CaptureStdout();
    std::cout << test;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ( output, "64 64 64 \n" );
}

GTEST_API_ int main( int argc, char ** argv ) {
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
