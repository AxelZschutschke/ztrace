/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
    MaterialTestImpl()
        : Material()
    {
    }
    bool scatterView(Ray const& rayIn ATT_UNUSED,
                     TraceData const& traceData ATT_UNUSED,
                     Vector& attenuation ATT_UNUSED,
                     Ray& scattered ATT_UNUSED) const
    {
        return true;
    }
    bool scatterView(Ray const& rayIn ATT_UNUSED,
                     TraceData const& traceData ATT_UNUSED,
                     Vector& attenuation1 ATT_UNUSED,
                     Ray& scattered1 ATT_UNUSED,
                     Vector& attenuation2 ATT_UNUSED,
                     Ray& scattered2 ATT_UNUSED, Real& weight1) const
    {
        return true;
    }
    bool scatterLight(Ray const& viewRayIn ATT_UNUSED,
                      Ray const& lightRayIn ATT_UNUSED,
                      TraceData const& traceData ATT_UNUSED,
                      Vector& attenuation ATT_UNUSED) const
    {
        return true;
    }
};
}

using Ray = ztrace::Ray;
using Vector = ztrace::Vector;
using Material = ztrace::MaterialTestImpl;
using TraceData = ztrace::TraceData;

TEST(Material_test, dummyConstructor) { Material test{}; }

TEST(Material_test, reflect_n) { Material test{}; }


GTEST_API_ int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
