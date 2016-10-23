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
/*! \brief returns a direction vector around (0,0,0) randomly scattered in unit-sphere
 *
 *  its length, by definition, is 0 < len < 1 and random
 *  \returns random direction vector
 */

Vector const randomScatterUnitSphere();
/*! \brief returns a direction vector around (0,0,0) randomly scattered in unit-circle
 *
 *  its length, by definition, is 0 < len < 1 and random
 *  the vector is returned in cylinder coordinates [R,phi,0.0]
 *  \returns random direction vector
 */
Vector const randomScatterCylinderCoordinate();

/*! \brief returns the direction after reflection on surface
 *
 *  length of reflected vector equals the inbound vector
 *  \param[in] in inbound vector to be reflected
 *  \param[in] normal surface normal at position of reflection
 *  \returns reflected vector
 */
Vector const reflect(Vector const& in, Vector const& normal);

} // namespace ztrace

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Implementation
//
ztrace::Vector const ztrace::randomScatterUnitSphere()
{
    Vector result;
    do {
        result = 2. * Vector{random(), random(), random()} - unit;
    } while (result.len() > 1.);
    return result;
}

ztrace::Vector const ztrace::randomScatterCylinderCoordinate()
{
    return Vector{random(), random() * 2. * M_PI, 0.0};
}

ztrace::Vector const ztrace::reflect(Vector const& in, Vector const& normal)
{
    return Vector{in - 2. * dot(in, normal) * normal};
}

#endif // ZTRACE_UTILS_H
