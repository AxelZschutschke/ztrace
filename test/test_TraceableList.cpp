/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <memory>
#include <gtest/gtest.h>
#include "Traceable.h"
#include "TraceableList.h"
#include "Sphere.h"

using ztrace::Int;
using ztrace::Real;

using Ray = ztrace::Ray;
using Vector = ztrace::Vector;
using Sphere = ztrace::Sphere;
using Traceable = ztrace::Traceable;
using TraceData = ztrace::TraceData;
using TraceableList = ztrace::TraceableList;

using SpherePtr = std::shared_ptr<Sphere>;

static Real const tol = (Real) 0.001;

TEST(TraceableList_test, constructor_dummy)
{
    TraceableList test{};
    EXPECT_EQ(test.begin(), test.end());
}

TEST(TraceableList_test, add)
{
    TraceableList test{};
    test.add(std::make_shared<Sphere>(Vector{(Real) 0.5, (Real) 0.75, (Real) 0.85}, (Real) 1.));
    SpherePtr mySphere = std::dynamic_pointer_cast<Sphere, Traceable>(*(test.begin()));
    EXPECT_NEAR(mySphere->center().x(), 0.5, tol);
    EXPECT_NEAR(mySphere->center().y(), 0.75, tol);
    EXPECT_NEAR(mySphere->center().z(), 0.85, tol);
    EXPECT_NEAR(mySphere->radius(), 1.0, tol);
}

TEST(TraceableList_test, hit)
{
    TraceableList test{};
    test.add(std::make_shared<Sphere>(Vector{0.5, 0.5, 1.5}, 1.));
    Ray ray({0.5, 0.5, 0.}, {0., 0., 1.});
    TraceData traceData{};

    bool gotHit = test.hit(ray, 0., 100., traceData);

    EXPECT_EQ(gotHit, true);
    // trace data filled with info about hit position
    EXPECT_EQ(traceData.positionOnRay, 0.5);
    EXPECT_EQ(traceData.point.x(), 0.5);
    EXPECT_EQ(traceData.point.y(), 0.5);
    EXPECT_EQ(traceData.point.z(), 0.5);
}

TEST(TraceableList_test, noHit)
{
    TraceableList test{};
    test.add(std::make_shared<Sphere>(Vector{0.5, 0.5, 1.5}, 1.));
    Ray ray({1.5, 1.5, 0.}, {0., 0., 1.});
    TraceData traceData{};

    bool gotHit = test.hit(ray, 0., 100., traceData);

    EXPECT_EQ(gotHit, false);
    // trace data filled with info about hit position
    EXPECT_EQ(traceData.positionOnRay, 0.0);
    EXPECT_EQ(traceData.point.x(), 0.0);
    EXPECT_EQ(traceData.point.y(), 0.0);
    EXPECT_EQ(traceData.point.z(), 0.0);
}

TEST(TraceableList_test, twoObjects_firstHit)
{
    TraceableList test{};
    test.add(std::make_shared<Sphere>(Vector{0.5, 0.5, 1.5}, 1.));
    test.add(std::make_shared<Sphere>(Vector{0.5, 0.5, 3.5}, 1.));
    Ray ray({0.5, 0.5, 0.}, {0., 0., 1.});
    TraceData traceData{};

    bool gotHit = test.hit(ray, 0., 100., traceData);

    EXPECT_EQ(gotHit, true);
    // trace data filled with info about hit position
    EXPECT_EQ(traceData.positionOnRay, 0.5);
    EXPECT_EQ(traceData.point.x(), 0.5);
    EXPECT_EQ(traceData.point.y(), 0.5);
    EXPECT_EQ(traceData.point.z(), 0.5);
}

TEST(TraceableList_test, twoObjects_noneHit)
{
    TraceableList test{};
    test.add(std::make_shared<Sphere>(Vector{0.5, 0.5, 1.5}, 1.));
    test.add(std::make_shared<Sphere>(Vector{0.5, 0.5, 3.5}, 1.));
    Ray ray({1.5, 1.5, 0.}, {0., 0., 1.});
    TraceData traceData{};

    bool gotHit = test.hit(ray, 0., 100., traceData);

    EXPECT_EQ(gotHit, false);
    // trace data filled with info about hit position
    EXPECT_EQ(traceData.positionOnRay, 0.0);
    EXPECT_EQ(traceData.point.x(), 0.0);
    EXPECT_EQ(traceData.point.y(), 0.0);
    EXPECT_EQ(traceData.point.z(), 0.0);
}

TEST(TraceableList_test, twoObjects_secondHit)
{
    TraceableList test{};
    test.add(std::make_shared<Sphere>(Vector{0.5, 0.5, 1.5}, 1.));
    test.add(std::make_shared<Sphere>(Vector{1.5, 1.5, 3.5}, 1.));
    Ray ray({1.5, 1.5, 0.}, {0., 0., 1.});
    TraceData traceData{};

    bool gotHit = test.hit(ray, 0., 100., traceData);

    EXPECT_EQ(gotHit, true);
    // trace data filled with info about hit position
    EXPECT_EQ(traceData.positionOnRay, 2.5);
    EXPECT_EQ(traceData.point.x(), 1.5);
    EXPECT_EQ(traceData.point.y(), 1.5);
    EXPECT_EQ(traceData.point.z(), 2.5);
}

GTEST_API_ int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
