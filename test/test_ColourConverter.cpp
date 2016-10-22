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

template <Int colourRange>
using ColourConverter = ztrace::ColourConverter<colourRange>;

TEST(Colour_test, constructor_Real_Gray)
{
    ColourConverter<128> test{0.25};
    EXPECT_EQ(test.red(), 32);
    EXPECT_EQ(test.green(), 32);
    EXPECT_EQ(test.blue(), 32);
}

TEST(Colour_test, constructor_Real)
{
    ColourConverter<128> test{(Real)0.25,(Real)0.5,(Real)-0.2};
    EXPECT_EQ(test.red(), 32);
    EXPECT_EQ(test.green(), 64);
    EXPECT_EQ(test.blue(), 0);
}

TEST(Colour_test, constructor_Int)
{
    ColourConverter<256> test{(Int)128,(Int)256,(Int)32};
    EXPECT_EQ(test.red(), 128);
    EXPECT_EQ(test.green(), 256);
    EXPECT_EQ(test.blue(), 32);
}

TEST(Colour_test, constructor_Vector )
{
    ztrace::Vector colourVector{0.5,-0.5,1.5};
    ColourConverter<128> test{colourVector};
    EXPECT_EQ(test.red(), 64);
    EXPECT_EQ(test.green(), 0);
    EXPECT_EQ(test.blue(), 128);
}

TEST(Colour_test, setRGB_Int )
{
    ColourConverter<128> test{};
    test.setRGB((Int) 10, (Int) 200, (Int) -10 );
    EXPECT_EQ(test.red(), 10);
    EXPECT_EQ(test.green(), 128);
    EXPECT_EQ(test.blue(), 0);
}

TEST(Colour_test, setRGB_Real )
{
    ColourConverter<128> test{};
    test.setRGB((Real)0.5, (Real)1.5, (Real)-0.5);
    EXPECT_EQ(test.red(), 64);
    EXPECT_EQ(test.green(), 128);
    EXPECT_EQ(test.blue(), 0);
}

TEST(Colour_test, setRGB_Vector )
{
    ztrace::Vector colourVector{0.5,-0.5,1.5};
    ColourConverter<128> test{};
    test.setRGB(colourVector );
    EXPECT_EQ(test.red(), 64);
    EXPECT_EQ(test.green(), 0);
    EXPECT_EQ(test.blue(), 128);
}

TEST(Colour_test, toStdout)
{
    ColourConverter<64> test{};
    test.setRGB((Real)0.25, (Real)0.5, (Real)0.75);
    testing::internal::CaptureStdout();
    std::cout << test;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "16 32 48");
}

GTEST_API_ int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
