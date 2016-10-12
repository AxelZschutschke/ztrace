#ifndef ZTRACE_CREATE_AMBIENT_LIGHTS_H
#define ZTRACE_CREATE_AMBIENT_LIGHTS_H

#include "Types.h"
#include "TraceableList.h"
#include "Light.h"
#include "LightList.h"
#include "Ray.h"

namespace ztrace {

    // \todo handle glass
    //
    bool getNewLightPosition( Light const & current, Traceable const & world, TraceData & newPosition ) {
        Ray forward;
        Size counter = 0;
        do {
          current.emitForwardRay( forward );
          ++counter;
        } while( ! world.hit( forward, 0.01, current.intensity(), newPosition ) and counter < 20 );
        return true;
    }
    bool getIntensity( Light const & current, TraceData const & newPosition, Real minIntensity, Vector & intensityOut ) {
        ShadowRay backward;
        if( current.getShadowRay( newPosition.point, intensityOut, backward ) ) {
            if( intensityOut.len() > minIntensity ) {
                return true;
            }
        }
        return false;
    }

    void addLightsToList( LightList & list, Light const & current, Traceable const & world, Size maxDepth, Real minIntensity ) {
        TraceData newPoint;
        Vector    intensity;
        Real      distance;
        if( getNewLightPosition( current, world, newPoint ) ){
            if( getIntensity( current, newPoint, minIntensity, intensity ) ){
                if( newPoint.material != nullptr ) {
                    if( newPoint.material->getGloss().hasDiffusive() ) {
                        std::shared_ptr<Light> newLight;
                        newLight = std::make_shared<LightChildPointSource>( newPoint.point, newPoint.material->albedo() * intensity, current.intensity() / 2., newPoint.positionOnRay );
                        list.add( newLight );
                        addLightsToList( list, *newLight, world, maxDepth-1, minIntensity );
                    }
                }
            }
        }
    }
    LightList createAmbientLights( LightList const & oldLights, 
            Traceable const & world, 
            Size numberPerLight, 
            Size maxDepth = 20, 
            Real minIntensity = 0.1 ) 
    {
        LightList newLights;
        
        for( auto light: oldLights ) {
            newLights.add( light );
            Light & current = *light;
            for( Size i = 0; i < numberPerLight; i++ )
            {
                addLightsToList( newLights, current, world, maxDepth, minIntensity );
            }
        }
        return newLights;
    }
}

#endif
