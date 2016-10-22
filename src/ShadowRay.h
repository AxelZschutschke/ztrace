/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZTRACE_SHADOW_RAY_H
#define ZTRACE_SHADOW_RAY_H

#include "Ray.h"

namespace ztrace
{

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Declaration
//

/*! \brief Ray containing specular switch, whether or not it may create specular lighting
 *
 * basic Ray extended by specular flag (defaults to 0), used for forward Ray tracing,
 * connecting light-
 * source and point of view. If Ray.specular == true, we get a specular lighting on
 * objects, this is only
 * true for real light sources. Light sources which are seen by reflection only (second
 * generation light
 * sources) may not create specular lighting
 */
class ShadowRay : public Ray
{
public:
    ShadowRay();
    ShadowRay(Vector const& A, Vector const& B, bool specular = false);
    ShadowRay(Vector const&& A, Vector const&& B, bool specular = false);
    ShadowRay(ShadowRay const& rhs, bool specular = false);
    ShadowRay(ShadowRay const&& rhs, bool specular = false);
    Ray const& operator=(ShadowRay const& rhs);
    Ray const& operator=(ShadowRay const&& rhs);

    bool& specular() { return specular_; }
    bool const& specular() const { return specular_; }

private:
    bool specular_;
};

} // namespace ztrace

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Implementation
//

ztrace::ShadowRay::ShadowRay()
    : Ray()
    , specular_()
{
}
ztrace::ShadowRay::ShadowRay(Vector const& A, Vector const& B, bool specular)
    : Ray(A, B)
    , specular_(specular)
{
}
ztrace::ShadowRay::ShadowRay(Vector const&& A, Vector const&& B, bool specular)
    : Ray(A, B)
    , specular_(specular)
{
}
ztrace::ShadowRay::ShadowRay(ShadowRay const& rhs, bool specular)
    : Ray(rhs)
    , specular_(specular)
{
}
ztrace::ShadowRay::ShadowRay(ShadowRay const&& rhs, bool specular)
    : Ray(rhs)
    , specular_(specular)
{
}
ztrace::Ray const& ztrace::ShadowRay::operator=(ShadowRay const& rhs)
{
    Ray::operator=(rhs);
    specular_ = rhs.specular_;
    return *this;
}
ztrace::Ray const& ztrace::ShadowRay::operator=(ShadowRay const&& rhs)
{
    Ray::operator=(rhs);
    specular_ = rhs.specular_;
    return *this;
}

#endif // ZTRACE_SHADOW_RAY_H
