/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZTRACE_UTILS_H
#define ZTRACE_UTILS_H

#include "Random.h"
#include "Types.h"
#include "Vector.h"

namespace ztrace
{
Vector const randomScatter()
{
    Vector result;
    do {
        result = 2. * Vector{random(), random(), random()} - unit;
    } while (result.len() > 1.);

    return Vector{result};
}
Vector const reflect(Vector const& in, Vector const& normal)
{
    return Vector{in - 2 * dot(in, normal) * normal};
}
}

#endif
