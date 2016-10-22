/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZTRACE_GLOSS_H
#define ZTRACE_GLOSS_H

#include "Vector.h"
#include "Ray.h"
#include "Traceable.h"

namespace ztrace
{
class Gloss
{
public:
    Gloss(bool diffusive, bool specular, bool transmission,
          Vector const& diffusiveColour = {0., 0., 0.}, Real const& specularHardness = 1.,
          Vector const& specularColour = {0., 0., 0.},
          Vector const& transmissivity = {0., 0., 0.})
        : diffusive_(diffusive)
        , specular_(specular)
        , transmission_(transmission)
        , diffusiveColour_(diffusiveColour)
        , specularColour_(specularColour)
        , specularHardness_(specularHardness)
        , transmissiveColour_(transmissivity)
    {
    }

    bool intensity(ShadowRay rayLightIn, TraceData const& traceData, Vector& attenuation) const
    {
        Vector lightIn = rayLightIn.direction();
        lightIn.makeUnitVector();
        lightIn *= -1;
        attenuation = zero;
        if (diffusive_) {
            Real angle = dot(lightIn, traceData.normal);
            angle = angle < 0. ? 0. : angle;
            attenuation += angle * diffusiveColour_;
        }
        if (specular_ && rayLightIn.specular()) {
            Real angle = dot(lightIn, traceData.reflection);
            angle = angle < 0. ? 0. : angle;
            attenuation += pow(angle, specularHardness_) * specularColour_;
        }
        attenuation.limitMax();
        return true;
    }

    bool hasDiffusive() const { return diffusive_; }
    bool hasSpecular() const { return specular_; }
    bool hasTransmission() const { return transmission_; }

    Real fuzz() const { return 1. / specularHardness_; }

    Vector const& albedo() const { return diffusiveColour_; }
    Vector const& transmissivity() const { return transmissiveColour_; }
    Vector const& reflectivity() const { return specularColour_; }

private:
    bool diffusive_;
    bool specular_;
    bool transmission_;
    Vector diffusiveColour_;
    Vector specularColour_;
    Real specularHardness_;
    Vector transmissiveColour_;
};

static Gloss const perfectBlack(true, false, false);
}

#endif
