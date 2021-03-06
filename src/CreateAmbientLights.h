/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZTRACE_CREATE_AMBIENT_LIGHTS_H
#define ZTRACE_CREATE_AMBIENT_LIGHTS_H

#include "Types.h"
#include "Ray.h"
#include "Material.h"
#include "TraceableList.h"
#include "Light.h"
#include "LightList.h"

namespace ztrace
{

// \todo handle glass
// \todo exceptions!
//
bool getNewLightPosition(Light const& current, Traceable const& world,
                         TraceData& traceData, Ray& rayOut)
{
    Size counter = 0;
    do {
        current.emitForwardRay(rayOut);
        ++counter;
    } while (!world.hit(rayOut, (Real) 0.01, current.intensity(), traceData) && counter < 20);
    if (counter < 20) {
        return true;
    }
    return false;
}

bool getIntensity(Light const& current, TraceData const& newPosition, Vector& intensityOut)
{
    ShadowRay backward;
    if (current.getShadowRay(newPosition.point, intensityOut, backward)) {
        return true;
    }
    return false;
}
bool checkIntensity(Vector const& intensity, Real const& minIntensity)
{
    if (intensity.len() > minIntensity) {
        return true;
    }
    return false;
}

bool addFurtherLightsToList(LightList& list, Light const& current, Traceable const& world,
                            Size maxDepth, Real minIntensity)
{
    TraceData traceData;
    Vector intensity;
    Ray forwardRay;
    if (getNewLightPosition(current, world, traceData, forwardRay)) {
        if (getIntensity(current, traceData, intensity) &&
            checkIntensity(intensity, minIntensity)) {
            Vector attenuation;
            Ray scatteredRay;
            if ((traceData.material->gloss().hasSpecular() ||
                 traceData.material->gloss().hasTransmission())) {
                if (traceData.material->scatterView(forwardRay, traceData, attenuation,
                                                    scatteredRay)) {
                    std::shared_ptr<Light> newLight;
                    newLight = std::make_shared<LightChildSpot>(
                        scatteredRay.origin(), attenuation * intensity,
                        current.intensity(), scatteredRay.direction(),
                        traceData.material->fuzz(), traceData.positionOnRay);
                    list.add(newLight);
                    addFurtherLightsToList(list, *newLight, world, maxDepth - 1,
                                           minIntensity);
                }
            }
            if (traceData.material->gloss().hasDiffusive()) {
                std::shared_ptr<Light> newLight;
                newLight = std::make_shared<LightChildPointSource>(
                    traceData.point,
                    traceData.material->gloss().albedo() * intensity / (Real) 2. / (Real) 3.1415,
                    current.intensity() / (Real) 2. / (Real) 3.1416, traceData.positionOnRay);
                list.add(newLight);
            }
        }
    }
    return false;
}
bool addSpotToList(LightList& list, Light const& current, Traceable const& world,
                   Size maxDepth, Real minIntensity)
{
    TraceData traceData;
    Vector intensity;
    Ray forwardRay;
    if (getNewLightPosition(current, world, traceData, forwardRay)) {
        if (getIntensity(current, traceData, intensity) &&
            checkIntensity(intensity, minIntensity)) {
            Vector attenuation;
            Ray scatteredRay;
            if ((traceData.material->gloss().hasSpecular() ||
                 traceData.material->gloss().hasTransmission())) {
                if (traceData.material->scatterView(forwardRay, traceData, attenuation,
                                                    scatteredRay)) {
                    std::shared_ptr<Light> newLight;
                    newLight = std::make_shared<LightChildSpot>(
                        scatteredRay.origin(), attenuation, current.intensity(),
                        scatteredRay.direction(),
                        traceData.material->fuzz() * 10., // fuzz increased for light-rays
                                                          // -> important to see some
                                                          // effect!
                        traceData.positionOnRay);
                    list.add(newLight);
                    addFurtherLightsToList(list, *newLight, world, maxDepth - 1,
                                           minIntensity);
                    return true;
                }
            }
        }
    }
    return false;
}

LightList createAmbientLights(LightList const& oldLights, Traceable const& world,
                              Size numberPerLight, Size maxDepth = 20,
                              Real minIntensity = 0.1)
{
    LightList newLights;

    for (auto light : oldLights) {
        newLights.add(light);
        Light& current = *light;
        for (Size i = 0; i < numberPerLight; ++i) {
            Size counter = 0;
            static Size const counterMax = 2500; // try to find "interesting" objects
            while (!addSpotToList(newLights, current, world, maxDepth, minIntensity) &&
                   counter < counterMax) {
                ++counter;
            }
            if (counter == counterMax) {
                addFurtherLightsToList(newLights, current, world, maxDepth, minIntensity);
            }
        }
    }
    return newLights;
}
}

#endif
