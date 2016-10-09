#include <iostream>
#include "Colour.h"
#include "Image.h"
#include "Vector.h"
#include "Ray.h"
#include "PPMWriter.h"

using Vector = ztrace::Vector;
using Ray = ztrace::Ray;
using Colour = ztrace::Colour<80>;
using Image = ztrace::Image<Colour>;
using PPMWriter = ztrace::PPMWriter<Image>;

Real hitSphere( Vector const & center, Real const & radius, Ray const & ray ){
    Vector oc = ray.origin() - center;
    Real a = dot( ray.direction(), ray.direction() );
    Real b = 2.0 * dot( oc, ray.direction() );
    Real c = dot( oc, oc ) - radius * radius;
    Real discriminant = (b * b - 4. * a * c );
    Real result;
    if (discriminant <= 0.) {
        result = -1.0;
    } else {
        result = (-b - sqrt(discriminant)) / (2.0 * a);
    }
    return result;

}
Colour const setBGColour( Ray const & ray ){
    Colour colour;
    Vector center( 0.0, 0.0, -1. );
    Real length = hitSphere( center, 0.4, ray );
    if ( length > 0.0 ){
    } else {
        Vector unit_direction = ray.direction();
        unit_direction.makeUnitVector();

        Real t = 0.5 * (unit_direction.y() + 1.0);
        Vector result = (1.0 - t) * Vector(1., 1., 1.) + t * Vector(0.5, 0.7, 1.0);
        colour.setRGB(result.x(), result.y(), result.z());
    }
    return colour;
}

int main()
{

    Int width  = 800;
    Int height = 400;

    Vector upperLeftCorner( -2.0, 1.0, -1.0 );
    Vector horizontal( 4.0, 0.0, 0.0 );
    Vector vertical( 0.0, -2.0, 0.0 );
    Vector origin( 0.0, 0.0, 0.0 );

    Image image( width, height );
    for( int y = 0; y < height; ++y ){
        for( int x = 0; x < width; ++x ){
            Vector xPos = (Real) x / (Real) width * horizontal;
            Vector yPos = (Real) y / (Real) height * vertical;
            Ray    ray( origin, upperLeftCorner + xPos + yPos );
            image.setPixel( x, y, setBGColour( ray ));
        }
    }
    PPMWriter writer;
    writer( "sample.ppm", image );
    return 0;
}