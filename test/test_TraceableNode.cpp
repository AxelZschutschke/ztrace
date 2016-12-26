/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Sphere.h"
#include "Traceable.h"
#include "TraceableNode.h"
#include <gtest/gtest.h>
#include <iostream>
#include <memory>

using ztrace::Int;
using ztrace::Real;

using Ray = ztrace::Ray;
using Vector = ztrace::Vector;
using Sphere = ztrace::Sphere;
using Traceable = ztrace::Traceable;
using TraceData = ztrace::TraceData;
using TraceableNode = ztrace::TraceableNode;

using SpherePtr = std::shared_ptr<Sphere>;

static Real const tolerance = (Real) 0.001;

TEST(TraceableNode_test, defaultContructor_singleObject)
{
    Traceable::SharedTraceableVector objects;
    objects.push_back(std::make_shared<Sphere>(Vector{0.5, 0., 0.}, 0.1));
    TraceableNode test(objects);
    TraceData traceData;

    // test direct hit
    EXPECT_TRUE(test.hit({{0., 0., 0.}, {1., 0., 0.}}, (Real) 0., (Real) 1.0, traceData));
    // test no hit
    EXPECT_FALSE(test.hit({{1., 0., 0.}, {1., 0., 0.}}, (Real) 0., (Real) 1.0, traceData));
    // test range too short
    EXPECT_FALSE(test.hit({{0., 0., 0.}, {1., 0., 0.}}, (Real) 0., (Real) 0.1, traceData));
    // test lower range to big
    EXPECT_FALSE(test.hit({{0., 0., 0.}, {1., 0., 0.}}, (Real) 1., (Real) 2.0, traceData));
}

TEST(TraceableNode_test, defaultContructor_twoObjects)
{
    Traceable::SharedTraceableVector objects;
    objects.push_back(std::make_shared<Sphere>(Vector{0.5, 0., 0.}, (Real) 0.1));
    objects.push_back(std::make_shared<Sphere>(Vector{0.5, 0.5, 0.}, (Real)  0.1));
    TraceableNode test(objects);
    TraceData traceData;

    // test direct hit #1
    EXPECT_TRUE(test.hit({{0., 0., 0.}, {1., 0., 0.}}, (Real) 0., (Real) 1.0, traceData));
    // test direct hit #2
    EXPECT_TRUE(test.hit({{0., 0.5, 0.}, {1., 0., 0.}}, (Real) 0., (Real) 1.0, traceData));
    // test no hit (shoot through middle)
    EXPECT_FALSE(test.hit({{0., 0.75, 0.}, {1., 0., 0.}}, (Real) 0., (Real) 1.0, traceData));
}

TEST(TraceableNode_test, boundingBoxSize_twoObjects)
{
    Traceable::SharedTraceableVector objects;
    objects.push_back(std::make_shared<Sphere>(Vector{0.5, 0., 0.}, (Real) 0.1));
    objects.push_back(std::make_shared<Sphere>(Vector{0.5, 0.5, 0.}, (Real) 0.1));
    TraceableNode test(objects);
    TraceData traceData;

    // test bounding box size (should contain exactlu both spheres)
    EXPECT_NEAR(test.boundingBox().min()[0], (Real) 0.4, tolerance);
    EXPECT_NEAR(test.boundingBox().min()[1], (Real)-0.1, tolerance);
    EXPECT_NEAR(test.boundingBox().min()[2], (Real)-0.1, tolerance);
    EXPECT_NEAR(test.boundingBox().max()[0], (Real) 0.6, tolerance);
    EXPECT_NEAR(test.boundingBox().max()[1], (Real) 0.6, tolerance);
    EXPECT_NEAR(test.boundingBox().max()[2], (Real) 0.1, tolerance);
}

TEST(TraceableNode_test, boundingBoxSize_fiveObjects)
{
    Traceable::SharedTraceableVector objects;
    objects.push_back(std::make_shared<Sphere>(Vector{0.5, 0., 0.}, (Real) 0.1));
    objects.push_back(std::make_shared<Sphere>(Vector{0.5, 0.5, 5.}, (Real) 0.1));
    objects.push_back(std::make_shared<Sphere>(Vector{0.5, -9.5, 0.5}, (Real) 0.1));
    objects.push_back(std::make_shared<Sphere>(Vector{3.5, 3.5, -0.5}, (Real) 1.0));
    objects.push_back(std::make_shared<Sphere>(Vector{-5.0, 0.0, 0.}, (Real) 4.0));
    TraceableNode test(objects);
    TraceData traceData;

    // test bounding box size (should contain exactlu both spheres)
    EXPECT_NEAR(test.boundingBox().min()[0], (Real)-9.0, tolerance); // #5
    EXPECT_NEAR(test.boundingBox().min()[1], (Real)-9.6, tolerance); // #3
    EXPECT_NEAR(test.boundingBox().min()[2], (Real)-4.0, tolerance); // #5
    EXPECT_NEAR(test.boundingBox().max()[0], (Real) 4.5, tolerance);  // #4
    EXPECT_NEAR(test.boundingBox().max()[1], (Real) 4.5, tolerance);  // #4
    EXPECT_NEAR(test.boundingBox().max()[2], (Real) 5.1, tolerance);  // #2
}
GTEST_API_ int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
