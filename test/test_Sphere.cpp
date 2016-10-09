//
// Created by Axel Zschutschke on 10/9/16.
//

#include "Sphere.h"
#include <iostream>
#include <gtest/gtest.h>

using ztrace::Int;
using ztrace::Real;

using Ray = ztrace::Ray;
using Vector = ztrace::Vector;
using Sphere = ztrace::Sphere;
using TraceData = ztrace::TraceData;

TEST( Sphere_test, constructor_dummy )
{
    Sphere test{ };
    EXPECT_EQ( test.center().len(), 0. );
    EXPECT_EQ( test.radius(), 0. );
}

TEST( Sphere_test, constructor_full )
{
    Sphere test({0.,0.,1.}, 0.5 );
    EXPECT_EQ( test.center().len(), 1. );
    EXPECT_EQ( test.radius(), 0.5 );
}

TEST( Sphere_test, checkHitInterval )
{
    EXPECT_EQ( Sphere::checkHitInterval( 0.5, 0., 1. ), true );
    EXPECT_EQ( Sphere::checkHitInterval( 1.5, 0., 1. ), false );
    EXPECT_EQ( Sphere::checkHitInterval( 1.5, 0., 1. ), false );
    EXPECT_EQ( Sphere::checkHitInterval(-0.5, 0., 1. ), false );
    EXPECT_EQ( Sphere::checkHitInterval(-0.5,-1., 1. ), true );
}

TEST( Sphere_test, hit )
{
    Sphere sphere({0.,0.,1.}, 0.5 );
    Ray ray( {0.,0.,0.}, {0.,0.,1.} );
    TraceData traceData{};

    bool gotHit = sphere.hit( ray, 0., 100., traceData );

    EXPECT_EQ( gotHit,  true );
    // trace data filled with info about hit position
    EXPECT_EQ( traceData.positionOnRay, 0.5 );
    EXPECT_EQ( traceData.point.x(), 0.0 );
    EXPECT_EQ( traceData.point.y(), 0.0 );
    EXPECT_EQ( traceData.point.z(), 0.5 );
}

TEST( Sphere_test, nohit )
{
    Sphere sphere({0.,0.,1.}, 0.5 );
    Ray ray( {1.,0.,0.}, {0.,0.,1.} );
    TraceData traceData{};

    bool gotHit = sphere.hit( ray, 0., 100., traceData );

    EXPECT_EQ( gotHit,  false );
    // trace data still unchanged (0)
    EXPECT_EQ( traceData.positionOnRay, 0.0 );
    EXPECT_EQ( traceData.point.x(), 0.0 );
    EXPECT_EQ( traceData.point.y(), 0.0 );
    EXPECT_EQ( traceData.point.z(), 0.0 );
}

TEST( Sphere_test, hitOutOfLimits )
{
    Sphere sphere({0.,0.,1.}, 0.5 );
    Ray ray( {0.,0.,0.}, {0.,0.,1.} );
    TraceData traceData{};

    bool gotHit = sphere.hit( ray, 0.0, 0.1, traceData );

    EXPECT_EQ( gotHit,  false );
    // trace data still unchanged (0)
    EXPECT_EQ( traceData.positionOnRay, 0.0 );
    EXPECT_EQ( traceData.point.x(), 0.0 );
    EXPECT_EQ( traceData.point.y(), 0.0 );
    EXPECT_EQ( traceData.point.z(), 0.0 );
}

GTEST_API_ int main( int argc, char ** argv ) {
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}