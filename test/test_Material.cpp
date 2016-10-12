//
// Created by Axel Zschutschke on 10/11/16.
//

#include "Types.h"
using ztrace::Int;
using ztrace::Real;

Real drand48()
{
    return 0.6;
}

#include "Material.h"
#include <iostream>
#include <gtest/gtest.h>


namespace ztrace
{
    class MaterialTestImpl : public Material
    {
        bool scatterView( Ray const & rayIn __attribute__((unused)), 
                TraceData const & traceData __attribute__((unused)), 
                Vector & attenuation __attribute__((unused)), 
                Ray & scattered __attribute__((unused)) ) const  
        { return true; }
        bool scatterLight( Ray const & viewRayIn __attribute__((unused)), 
                Ray const & lightRayIn __attribute__((unused)), 
                TraceData const & traceData __attribute__((unused)), 
                Vector & attenuation  __attribute__((unused))) const
        { return true; } 
    }
}

using Ray = ztrace::Ray;
using Vector = ztrace::Vector;
using Material = ztrace::MaterialTestImpl;
using TraceData = ztrace::TraceData;

TEST( Material_test, randomScatter )
{
    Material test{ };
    EXPECT_EQ( test.randomScatter( Vector{1.0,0.0,0.0}).x(), 1.2 );
    EXPECT_EQ( test.randomScatter( Vector{1.0,0.0,0.0}).y(), 0.2 );
    EXPECT_EQ( test.randomScatter( Vector{0.0,0.0,0.5}).z(), 0.7 );
}

TEST( Material_test, reflect_n)
{
    Material test{};
    EXPECT_EQ(  );
    EXPECT_EQ( test.radius(), 0.5 );
}

TEST( Material_test, checkHitInterval )
{
    EXPECT_EQ( Material::checkHitInterval( 0.5, 0., 1. ), true );
    EXPECT_EQ( Material::checkHitInterval( 1.5, 0., 1. ), false );
    EXPECT_EQ( Material::checkHitInterval( 1.5, 0., 1. ), false );
    EXPECT_EQ( Material::checkHitInterval(-0.5, 0., 1. ), false );
    EXPECT_EQ( Material::checkHitInterval(-0.5,-1., 1. ), true );
}

TEST( Material_test, hit )
{
    Material sphere({0.,0.,1.}, 0.5 );
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

TEST( Material_test, nohit )
{
    Material sphere({0.,0.,1.}, 0.5 );
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

TEST( Material_test, hitOutOfLimits )
{
    Material sphere({0.,0.,1.}, 0.5 );
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
