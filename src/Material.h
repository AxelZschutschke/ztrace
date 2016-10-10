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

    class Glass : public Material
    {
    public:
        Glass( ) 
            : albedo_(1.,1.,1.)
            , fuzz_(0.)
            , refractIndex_( 1.5 )
        {}
        Glass( Vector const & albedo, Real const & refractIndex = 1.5, Real const & fuzz = 0. )
            : albedo_( albedo )
            , fuzz_(fuzz)
            , refractIndex_( refractIndex )
        {
           fuzz_ = fuzz_ < 0. ? 0. : fuzz_;
           fuzz_ = fuzz_ > 1. ? 1. : fuzz_;
        }

        bool refract( Vector direction, Vector const & normal, Real const & refractIndex, Vector & refracted ) const {
            direction.makeUnitVector();
            Real steepness = dot( direction, normal );
            Real discriminant = 1.0 - refractIndex * refractIndex * ( 1. - steepness * steepness );
            if( discriminant > 0. ) {
                refracted = refractIndex * ( direction - normal * steepness ) - normal * sqrt( discriminant );
                return true;
            }
            return false;
        }
        Real  schlick( Real const & cosine, Real const & refractIndex ) const {
            Real r0 = ( 1. - refractIndex ) / ( 1. + refractIndex );
            r0 = r0 * r0;
            return r0 + ( 1. - r0 ) * pow(( 1. - cosine ), 5 );
        }
        bool scatter( Ray const & rayIn, TraceData & traceData, Vector & attenuation, Ray & scattered ) const {
            Vector outward_normal;
            Real refractIndex;
            Real reflectionProbability;
            Real cosine;
            Vector refracted;
            if( dot( rayIn.direction(), traceData.normal ) > 0. ) { 
                outward_normal = -traceData.normal;
                refractIndex = refractIndex_;
                cosine = refractIndex * dot( rayIn.direction(), traceData.normal ) / rayIn.direction().len();
            }else{
                outward_normal = traceData.normal;
                refractIndex = 1. / refractIndex_;
                cosine = -dot( rayIn.direction(), traceData.normal ) / rayIn.direction().len();
            }
            if( refract( rayIn.direction(), outward_normal, refractIndex, refracted ) ) {
                reflectionProbability = schlick( cosine, refractIndex );
            }else{
                reflectionProbability = 1.0;
            }
            if( drand48() < reflectionProbability )
            {
                Vector reflected = reflect( rayIn.direction(), traceData.normal );
                scattered = Ray( traceData.point, reflected );
                attenuation = Vector{ 1., 1., 1. };
            }else{
                scattered = Ray( traceData.point, refracted );
                attenuation = albedo_;
            }
            return true;
        }
    private:
        Vector albedo_; // Colour
        Real   fuzz_;   // How much scattering
        Real   refractIndex_; // Refracting index
    };
}

#endif //ZTRACER_MATERIAL_H
