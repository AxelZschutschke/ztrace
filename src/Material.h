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
        Vector const randomScatter( Vector const & normal ) const {
          Vector result;
          Vector unit{1.,1.,1.};
          do {
              result = 2. * Vector{drand48(), drand48(), drand48()} - unit;
          } while( result.len() > 1. );

          return Vector{ normal * 1.1 + result };
        }
        Vector const reflect( Vector const & normal, Vector const & rayIn ) const {
            return Vector{ rayIn - 2 * dot( rayIn, normal ) * normal };
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
            scattered = Ray{ traceData.point, randomScatter( traceData.normal ) };
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
            , fuzz_(0.)
        {}
        Metal( Vector const & albedo, Real fuzz = 0. )
            : albedo_( albedo )
            , fuzz_(fuzz)
        {
           fuzz_ = fuzz_ < 0. ? 0. : fuzz_;
           fuzz_ = fuzz_ > 1. ? 1. : fuzz_;
        }

        bool scatter( Ray const & rayIn, TraceData & traceData, Vector & attenuation, Ray & scattered ) const {
            scattered = Ray{ traceData.point, reflect( traceData.normal, rayIn.direction() ) + fuzz_ * randomScatter( traceData.normal ) };
            attenuation = albedo_;
            return dot( scattered.direction(), traceData.normal ) > 0.;
        }
    private:
        Vector albedo_; // Colour
        Real   fuzz_;   // How much scattering
    };
}

#endif //ZTRACER_MATERIAL_H
