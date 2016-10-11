#ifndef ZTRACE_LIGHT_H
#define ZTRACE_LIGHT_H

#include "Vector.h"
#include "Ray.h"

namespace ztrace {

    class Light {
    public:
        using Colour = ztrace::Vector;

        virtual bool getShadowRay( Vector const & targetSpatialPosition, Colour & gainOut, Ray & shadowRayOut ) const = 0;
    };


    class LightPointSource : public Light
    {
    public:
        LightPointSource()
            : Light()
            , position_()
            , colour_()
            , intensity_()
        {}
        LightPointSource( Vector const & position, Colour const & colour, Real intesity )
            : Light()
            , position_(position)
            , colour_(colour)
            , intensity_(intesity)
        {}

        bool getShadowRay( Vector const & targetSpatialPosition, Colour & gainOut, Ray & shadowRayOut ) const {
            Vector direction = targetSpatialPosition - position_;
            Real   distance = direction.len();
            shadowRayOut = Ray{ position_, direction };
            gainOut = colour_ * intensityOverDistance( shadowRayOut.direction(), distance );
            return true;
        }

    private:
        Real intensityOverDistance( Vector const & direction __attribute__((unused)), Real const & distance ) const {
            return intensity_ / distance / distance;
        }

        Vector position_;
        Colour colour_;
        Real   intensity_;
    };
}

#endif
