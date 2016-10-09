#include <iostream>
#include "Colour.h"
#include "Image.h"
#include "Vector.h"
#include "TraceableList.h"
#include "Sphere.h"
#include "Ray.h"
#include "Camera.h"
#include "PPMWriter.h"
#include <stdlib.h>


using Vector = ztrace::Vector;
using Ray = ztrace::Ray;
using Colour = ztrace::Colour<80>;
using Image = ztrace::Image<Colour>;
using PPMWriter = ztrace::PPMWriter<Image>;
using TraceableList = ztrace::TraceableList;
using TraceData = ztrace::TraceData;
using Sphere = ztrace::Sphere;
using Camera = ztrace::Camera;

Colour const setBGColour( Ray const & ray, TraceableList & traceable  ){
    Colour colour;
    TraceData traceData;
    bool hitSomething = traceable.hit( ray, 0., 100., traceData );
    if ( hitSomething ){
        colour.setRGB( 1., 0., 0. );
    } else {
        Vector unit_direction = ray.direction();
        unit_direction.makeUnitVector();

        Real t = 0.5 * (unit_direction.y() + 1.0);
        Vector result = (1.0 - t) * Vector(1., 1., 1.) + t * Vector(0.5, 0.7, 1.0);
        colour.setRGB(result.x(), result.y(), result.z());
    }
    return colour;
}

Real const position( Int const & x, Int const & screenWidth ) {
    return ((Real) x + (drand48() - 0.5) ) / ((Real) screenWidth);
}

int main()
{

    Int width  = 900;
    Int height = 450;
    Int antiAliasing = 4;

    Image image( width, height );

    TraceableList traceables{};
    traceables.add( std::make_shared<Sphere>(Vector{1.,1.,-1.5}, 0.25 ));
    traceables.add( std::make_shared<Sphere>(Vector{0.0,1.,-1.5}, 0.75 ));
    traceables.add( std::make_shared<Sphere>(Vector{0.,-101.,-1.5}, 100.5 ));

    Camera cam{};

    for( Int y = 0; y < height; ++y ){
        for( Int x = 0; x < width; ++x ){
            Real xPos, yPos;
            Colour colour;
            for( Int a = 0; a < antiAliasing; ++a ) {
                xPos = position( x, width );
                yPos = position( y, height);
                Ray    ray = cam.emitRay( xPos, yPos );
                colour += setBGColour(ray, traceables) / antiAliasing;
            }
            image.setPixel( x, y, colour);
        }
    }
    PPMWriter writer;
    writer( "sample.ppm", image );
    return 0;
}