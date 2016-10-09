//
// Created by Axel Zschutschke on 10/8/16.
//

#include "Component.h"
#include <iostream>
#include <gtest/gtest.h>

using ztrace::Int;
using ztrace::Real;

template<Int colourRange>
using Component = ztrace::Component<colourRange>;

TEST( Component_test, constructor_real )
{
    Component<128> test( 0.5 );
    EXPECT_EQ( test.getInt(), 64 );
    EXPECT_EQ( test.getReal(), 0.5 );
}

TEST( Component_test, constructor_int )
{
    Component<128> test(32);
    EXPECT_EQ( test.getInt(), 32 );
    EXPECT_EQ( test.getReal(), 0.25 );
}

TEST( Component_test, limit_range_upper )
{
    Component<128> test(132);
    EXPECT_EQ( test.getInt(), 128 );
    EXPECT_EQ( test.getReal(), 1.0 );
}

TEST( Component_test, limit_range_lower )
{
    Component<128> test(-20);
    EXPECT_EQ( test.getInt(), 0 );
    EXPECT_EQ( test.getReal(), 0.0 );
}

TEST( Component_test, invert )
{
    Component<128> test(-20);
    Component<128> test2 = test.invert();
    EXPECT_EQ( test2.getInt(), 128 );
    EXPECT_EQ( test2.getReal(), 1.0 );
}

TEST( Component_test, toStdout )
{
    Component<64> test(48);
    testing::internal::CaptureStdout();
    std::cout << test;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ( output, "48" );
}

GTEST_API_ int main( int argc, char ** argv ) {
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
