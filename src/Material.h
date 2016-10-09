//
// Created by Axel Zschutschke on 10/9/16.
//

#ifndef ZTRACER_MATERIAL_H
#define ZTRACER_MATERIAL_H

#include "Vector.h"
#include "Ray.h"
#include "Traceable.h"

namespace ztrace {

    class Material {
    public:
        Ray const randomScatter( Vector const & point, Vector const & normal ) const {
          Vector result;
          Vector unit{1.,1.,1.};
          do {
              result = 2. * Vector{drand48(), drand48(), drand48()} - unit;
          } while( result.len() > 1. );

          return Ray{ point, normal * 1.1 + result };
        }
        Ray const reflect( Vector const & point, Vector const & normal, Vector const & rayIn ) const {
            return Ray{ point, rayIn - 2 * dot( rayIn, normal ) * normal };
        }

        virtual bool scatter( Ray const & rayIn, TraceData & traceData, Vector & attenuation, Ray & scattered ) const = 0;
    };

    class Lambertian : public Material
    {
    public:
        Lambertian( ) 
            : albedo_()
        {}
        Lambertian( Vector const & albedo )
            : albedo_( albedo )
        {}

        virtual bool scatter( Ray const & rayIn __attribute__((unused)), TraceData & traceData, Vector & attenuation, Ray & scattered ) const {
            scattered = randomScatter( traceData.point, traceData.normal );
            attenuation = albedo_;
            return true;
        }
    private:
        Vector albedo_; // Colour
    };

    class Metal : public Material
    {
    public:
        Metal( ) 
            : albedo_(1.,1.,1.)
        {}
        Metal( Vector const & albedo )
            : albedo_( albedo )
        {}

        bool scatter( Ray const & rayIn, TraceData & traceData, Vector & attenuation, Ray & scattered ) const {
            scattered = reflect( traceData.point, traceData.normal, rayIn.direction() );
            attenuation = albedo_;
            return dot( scattered.direction(), traceData.normal ) > 0.;
        }
    private:
        Vector albedo_; // Colour
    };
}

#endif //ZTRACER_MATERIAL_H
