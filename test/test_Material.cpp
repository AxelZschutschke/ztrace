#define ZTRACE_TESTING

#include "Random.h"
#include "Types.h"

using ztrace::Int;
using ztrace::Real;

#include "Material.h"
#include <iostream>
#include <gtest/gtest.h>


namespace ztrace
{
    class MaterialTestImpl : public Material
    {
        public:
        MaterialTestImpl() : Material() {}
        bool scatterView( Ray const & rayIn __attribute__((unused)), 
                TraceData const & traceData __attribute__((unused)), 
                Vector & attenuation __attribute__((unused)), 
                Ray & scattered __attribute__((unused)) ) const  
        { return true; }
        bool scatterView( Ray const & rayIn __attribute__((unused)), 
                TraceData const & traceData __attribute__((unused)), 
                Vector & attenuation1 __attribute__((unused)), 
                Ray & scattered1 __attribute__((unused)),  
                Vector & attenuation2 __attribute__((unused)), 
                Ray & scattered2 __attribute__((unused)), Real & weight1 ) const  
        { return true; }
        bool scatterLight( Ray const & viewRayIn __attribute__((unused)), 
                Ray const & lightRayIn __attribute__((unused)), 
                TraceData const & traceData __attribute__((unused)), 
                Vector & attenuation  __attribute__((unused))) const
        { return true; } 
    };
}

using Ray = ztrace::Ray;
using Vector = ztrace::Vector;
using Material = ztrace::MaterialTestImpl;
using TraceData = ztrace::TraceData;

TEST( Material_test, dummyConstructor)
{
    Material test{ };
}

TEST( Material_test, reflect_n)
{
    Material test{};
}


GTEST_API_ int main( int argc, char ** argv ) {
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
