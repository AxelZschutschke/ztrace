/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// make random() return const/testable value all the time
#define ZTRACE_TESTING

#include "Random.h"
#include "Vector.h"
#include "Utils.h"

#include <iostream>
#include <gtest/gtest.h>

using ztrace::Int;
using ztrace::Real;

using Vector = ztrace::Vector;

static Real const tol = 0.001;

TEST(Utils_test, randomScatterUnitSphere)
{
    Vector test = ztrace::randomScatterUnitSphere();
    EXPECT_LE(test.len(), 1.);
    EXPECT_NEAR(test.x(), 2. * ZTRACE_TESTING_RAND - 1., tol);
    EXPECT_NEAR(test.y(), 2. * ZTRACE_TESTING_RAND - 1., tol);
    EXPECT_NEAR(test.z(), 2. * ZTRACE_TESTING_RAND - 1., tol);
}

TEST(Utils_test, randomScatterCylinderCoordinates)
{
    Vector test = ztrace::randomScatterCylinderCoordinate();
    EXPECT_NEAR(test.x(), ZTRACE_TESTING_RAND, tol);
    EXPECT_NEAR(test.y(), ZTRACE_TESTING_RAND * 2.0 * M_PI,
                tol); // random angle on circle
    EXPECT_NEAR(test.z(), 0.0, tol);
}

TEST(Utils_test, reflect_normal)
{
    Vector a(2., 0., 0.);
    Vector normal(-1., 0., 0.);
    Vector test = reflect(a, normal);
    EXPECT_NEAR(test.len(), a.len(), tol);
    EXPECT_NEAR(test.x(), -2., tol);
    EXPECT_NEAR(test.y(), 0., tol);
    EXPECT_NEAR(test.z(), 0., tol);
}

TEST(Utils_test, reflect_angle)
{
    Vector a(2., 1., 0.);
    Vector normal(-1., 0., 0.);
    Vector test = reflect(a, normal);
    EXPECT_NEAR(test.len(), a.len(), tol);
    EXPECT_NEAR(test.x(), -2., tol);
    EXPECT_NEAR(test.y(), 1., tol);
    EXPECT_NEAR(test.z(), 0., tol);
}

TEST(Utils_test, reflect_tangent)
{
    Vector a(2., 0., 0.);
    Vector normal(0., 1., 0.);
    Vector test = reflect(a, normal);
    EXPECT_NEAR(test.len(), a.len(), tol);
    EXPECT_NEAR(test.x(), 2., tol);
    EXPECT_NEAR(test.y(), 0., tol);
    EXPECT_NEAR(test.z(), 0., tol);
}

GTEST_API_ int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
