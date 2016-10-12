//
// Created by Axel Zschutschke on 10/9/16.
//

#ifndef ZTRACE_MATERIAL_H
#define ZTRACE_MATERIAL_H

#ifndef ZTRACE_TESTING
#include <stdlib.h>
#endif

#include <cmath>
#include "Types.h"
#include "Vector.h"
#include "Ray.h"
#include "Traceable.h"
#include "Utils.h"
#include "Gloss.h"

namespace ztrace {

    class Material {
    public:
        Material( Gloss const & gloss )
            : gloss_(gloss)
        {}

        Gloss const & getGloss() const { return gloss_; }

        virtual bool scatterView( Ray const & rayIn, TraceData const & traceData, Vector & attenuation, Ray & scattered ) const = 0;
        virtual bool scatterLight( ShadowRay const & rayLightIn, TraceData const & traceData, Vector & attenuation ) const {
            return gloss_.intensity( rayLightIn, traceData, attenuation );
        }
        virtual Vector const & albedo() const { return gloss_.albedo(); }
        virtual Real fuzz() const { return gloss_.fuzz(); }
    private:
        Gloss const & gloss_;
    };

    class Lambertian : public Material
    {
    public:
        Lambertian( Gloss const & gloss )
            : Material( gloss )
        {}

        virtual bool scatterView( Ray const & rayIn __attribute__((unused)), TraceData const & traceData, Vector & attenuation, Ray & scattered ) const {
            scattered = Ray{ traceData.point, randomScatter( ) + traceData.normal };
            attenuation = albedo();
            return true;
        }
    };

    class Metal : public Material
    {
    public:
        Metal( Gloss const & gloss, Real fuzz = 0. )
            : Material( gloss )
            , fuzz_(fuzz)
        {
           fuzz_ = fuzz_ < 0. ? 0. : fuzz_;
           fuzz_ = fuzz_ > 1. ? 1. : fuzz_;
        }

        bool scatterView( Ray const & rayIn __attribute__((unused)), TraceData const & traceData, Vector & attenuation, Ray & scattered ) const {
            scattered = Ray{ traceData.point, traceData.reflection + fuzz_ * randomScatter( ) };
            attenuation = albedo();
            return dot( scattered.direction(), traceData.normal ) > 0.;
        }
    private:
        Real   fuzz_;   // How much scattering
    };

    class Glass : public Material
    {
    public:
        Glass( Gloss const & gloss, Real const & refractIndex = 1.5 )
            : Material( gloss )
            , refractIndex_( refractIndex )
        { }

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
        bool scatterView( Ray const & rayIn, TraceData const & traceData, Vector & attenuation, Ray & scattered ) const {
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
                attenuation = albedo();
            }
            return true;
        }
    private:
        Real   refractIndex_; // Refracting index
    };
}

#endif //ZTRACER_MATERIAL_H
