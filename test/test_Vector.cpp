//
// Created by Axel Zschutschke on 10/8/16.
//

#include "Vector.h"
#include <iostream>
#include <gtest/gtest.h>

using ztrace::Int;
using ztrace::Real;

using Vector = ztrace::Vector;

TEST( Vector_test, constructor_dummy )
{
    Vector test{ };
    EXPECT_EQ( test.len(), 0.0 );
}

TEST( Vector_test, constructor_full )
{
    Vector test{ 0.0, 2.0, 0.0 };
    EXPECT_EQ( test.len(), 2.0 );
}

TEST( Vector_test, makeUnitVector )
{
    Vector test{ 2.0, 1.0, 0.0 };
    test.makeUnitVector();
    EXPECT_DOUBLE_EQ( test.len(), 1.0 );
}

TEST( Vector_test, operatorAddAssign )
{
    Vector a{ 1.0, 2.0, 3.0 };
    Vector b{ 3.0, 2.0, 1.0 };
    a += b;
    EXPECT_DOUBLE_EQ( a.x(), 4.0 );
    EXPECT_DOUBLE_EQ( a.y(), 4.0 );
    EXPECT_DOUBLE_EQ( a.z(), 4.0 );
}
TEST( Vector_test, operatorSubAssign )
{
    Vector a{ 1.0, 2.0, 3.0 };
    Vector b{ 3.0, 2.0, 1.0 };
    a -= b;
    EXPECT_DOUBLE_EQ( a.x(), -2.0 );
    EXPECT_DOUBLE_EQ( a.y(),  0.0 );
    EXPECT_DOUBLE_EQ( a.z(),  2.0 );
}
TEST( Vector_test, operatorMulAssign )
{
    Vector a{ 1.0, 2.0, 3.0 };
    Vector b{ 3.0, 2.0, 1.0 };
    a *= b;
    EXPECT_DOUBLE_EQ( a.x(),  3.0 );
    EXPECT_DOUBLE_EQ( a.y(),  4.0 );
    EXPECT_DOUBLE_EQ( a.z(),  3.0 );
}
TEST( Vector_test, operatorDivAssign )
{
    Vector a{ 1.0, 2.0, 3.0 };
    Vector b{ 3.0, 2.0, 1.0 };
    a /= b;
    EXPECT_DOUBLE_EQ( a.x(),  1.0 / 3.0 );
    EXPECT_DOUBLE_EQ( a.y(),  1.0 );
    EXPECT_DOUBLE_EQ( a.z(),  3.0 );
}
TEST( Vector_test, operatorAdd )
{
    Vector a{ 1.0, 2.0, 3.0 };
    Vector b{ 3.0, 2.0, 1.0 };
    Vector c = a + b;
    EXPECT_DOUBLE_EQ( c.x(), 4.0 );
    EXPECT_DOUBLE_EQ( c.y(), 4.0 );
    EXPECT_DOUBLE_EQ( c.z(), 4.0 );
}
TEST( Vector_test, operatorSub )
{
    Vector a{ 1.0, 2.0, 3.0 };
    Vector b{ 3.0, 2.0, 1.0 };
    Vector c = a - b;
    EXPECT_DOUBLE_EQ( c.x(), -2.0 );
    EXPECT_DOUBLE_EQ( c.y(),  0.0 );
    EXPECT_DOUBLE_EQ( c.z(),  2.0 );
}
TEST( Vector_test, operatorMul )
{
    Vector a{ 1.0, 2.0, 3.0 };
    Vector b{ 3.0, 2.0, 1.0 };
    Vector c = a * b;
    EXPECT_DOUBLE_EQ( c.x(),  3.0 );
    EXPECT_DOUBLE_EQ( c.y(),  4.0 );
    EXPECT_DOUBLE_EQ( c.z(),  3.0 );
}
TEST( Vector_test, operatorDiv )
{
    Vector a{ 1.0, 2.0, 3.0 };
    Vector b{ 3.0, 2.0, 1.0 };
    Vector c = a / b;
    EXPECT_DOUBLE_EQ( c.x(),  1.0 / 3.0 );
    EXPECT_DOUBLE_EQ( c.y(),  1.0 );
    EXPECT_DOUBLE_EQ( c.z(),  3.0 );
}
TEST( Vector_test, abs )
{
    Vector a{ -1.0, -2.0, 3.0 };
    Vector c = a.abs();
    EXPECT_DOUBLE_EQ( c.x(),  1.0 );
    EXPECT_DOUBLE_EQ( c.y(),  2.0 );
    EXPECT_DOUBLE_EQ( c.z(),  3.0 );
}

TEST( Vector_test, toStdout )
{
    Vector test{ 0.25, 0.5, 0.75 };
    testing::internal::CaptureStdout();
    std::cout << test;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ( output, "0.25 0.5 0.75" );
}

GTEST_API_ int main( int argc, char ** argv ) {
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
