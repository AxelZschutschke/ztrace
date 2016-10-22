/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ShadowRay.h"
#include <iostream>
#include <gtest/gtest.h>

using ztrace::Int;
using ztrace::Real;

using ShadowRay = ztrace::ShadowRay;
using Vector = ztrace::Vector;

TEST(ShadowRay_test, constructor_dummy)
{
    ShadowRay test{};
    EXPECT_EQ(test.origin().len(), 0.);
    EXPECT_EQ(test.direction().len(), 0.);
    EXPECT_EQ(test.positionAtLength(1.0).len(), 0.);
}

TEST(ShadowRay_test, constructor_full)
{
    Vector a(2., 0., 0.);
    Vector b(1., 0., 0.);
    ShadowRay test{a, b};
    EXPECT_EQ(test.origin().len(), 2.);
    EXPECT_EQ(test.direction().len(), 1.);
    EXPECT_EQ(test.positionAtLength(1.0).len(), 3.);
}

TEST(ShadowRay_test, positionLength)
{
    Vector a(2., 0., 0.);
    Vector b(1., -1., 0.);
    ShadowRay test{a, b};
    Vector c = test.positionAtLength(2.);
    EXPECT_EQ(c.x(), 4.);
    EXPECT_EQ(c.y(), -2.);
    EXPECT_EQ(c.z(), 0.);
}

GTEST_API_ int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
