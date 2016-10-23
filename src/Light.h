/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZTRACE_LIGHT_H
#define ZTRACE_LIGHT_H

#include "Types.h"
#include "Vector.h"
#include "Utils.h"
#include "Ray.h"
#include "ShadowRay.h"
#include "Sphere.h"

namespace ztrace
{

class Light
{
public:
    using Colour = ztrace::Vector;

    virtual bool getShadowRay(Vector const& targetSpatialPosition, Colour& gainOut,
                              ShadowRay& shadowRayOut) const = 0;
    virtual bool emitForwardRay(Ray& shadowRayOut) const = 0;
    virtual Real intensityOverDistance(Real distance) const = 0;
    virtual Real const& intensity() const = 0;
};

class LightPointSource : public Light
{
public:
    LightPointSource()
        : Light()
        , position_()
        , colour_()
        , intensity_()
        , specular_(true)
    {
    }
    LightPointSource(Vector const& position, Colour const& colour, Real const& intesity,
                     bool specular = true)
        : Light()
        , position_(position)
        , colour_(colour)
        , intensity_(intesity)
        , specular_(specular)
    {
    }

    bool getShadowRay(Vector const& targetSpatialPosition, Colour& gainOut,
                      ShadowRay& shadowRayOut) const
    {
        Vector direction = targetSpatialPosition - position_;
        Real distance = direction.len();
        shadowRayOut = ShadowRay{position_, direction, specular_};
        gainOut = colour_ * intensityOverDistance(distance);
        return true;
    }
    bool emitForwardRay(Ray& shadowRayOut) const
    {
        shadowRayOut = Ray{position_, randomScatterUnitSphere()};
        return true;
    }
    virtual Real intensityOverDistance(Real distance) const
    {
        distance /= intensity_;
        distance += 1.;
        return 1. / distance / distance;
    }
    virtual Real const& intensity() const { return intensity_; }
private:
    Vector position_;
    Colour colour_;
    Real intensity_;
    bool specular_;
};

class LightSpot : public Light
{
public:
    LightSpot()
        : Light()
        , position_()
        , colour_()
        , intensity_()
        , direction_()
        , specular_(true)
    {
    }
    LightSpot(Vector const& position, Colour const& colour, Real const& intesity,
              Vector const& direction, Real const& size, bool specular = true)
        : Light()
        , position_(position)
        , colour_(colour)
        , intensity_(intesity)
        , direction_(direction)
        , size_(size)
        , specular_(specular)
    {
        size_ = size_ > 1. ? 1. : size_;
        direction_.makeUnitVector();
    }

    bool getShadowRay(Vector const& targetSpatialPosition, Colour& gainOut,
                      ShadowRay& shadowRayOut) const
    {
        Vector direction = targetSpatialPosition - position_;
        Real distance = direction.len();
        shadowRayOut = ShadowRay{position_, direction, specular_};
        direction /= distance;

        Real angle = dot(direction_, direction);
        angle *= angle;
        Real minimum = 1. - size_ * size_ * 0.78 * 0.78;
        if (angle >= minimum) { // angle via dot is 1 for zero size and 0 for size = 1
            gainOut = /*( angle - minimum ) / (1. - minimum) **/ colour_ *
                      intensityOverDistance(distance);
            return true;
        }
        return false;
    }
    bool emitForwardRay(Ray& shadowRayOut) const
    {
        shadowRayOut =
            Ray{position_, (direction_ + size_ * randomScatterUnitSphere()).makeUnitVector()};
        return true;
    }
    virtual Real intensityOverDistance(Real distance) const
    {
        distance /= intensity_;
        distance += 1.;
        return 1. / distance / distance;
    }
    virtual Real const& intensity() const { return intensity_; }

private:
    Vector position_;
    Colour colour_;
    Real intensity_;
    Vector direction_;
    Real size_;
    bool specular_;
};

class LightChildPointSource : public LightPointSource
{
public:
    LightChildPointSource()
        : LightPointSource()
        , offset_()
    {
    }
    LightChildPointSource(Vector const& position, Colour const& colour,
                          Real const& intesity, Real const& offset)
        : LightPointSource(position, colour, intesity, false)
        , offset_(offset)
    {
    }

    virtual Real intensityOverDistance(Real distance) const
    {
        distance += offset_;
        distance /= intensity();
        distance += 1.;
        return 1. / distance / distance;
    }

private:
    Real offset_;
};
class LightChildSpot : public LightSpot
{
public:
    LightChildSpot()
        : LightSpot()
        , offset_()
    {
    }
    LightChildSpot(Vector const& position, Colour const& colour, Real const& intesity,
                   Vector const& direction, Real const& size, Real const& offset)
        : LightSpot(position, colour, intesity, direction, size, false)
        , offset_(offset)
    {
    }

    virtual Real intensityOverDistance(Real distance) const
    {
        distance += offset_;
        distance /= intensity();
        distance += 1.;
        return 1. / distance / distance;
    }

private:
    Real offset_;
};
}

#endif
