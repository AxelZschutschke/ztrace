#include <iostream>
#include "ColourConverter.h"
#include "Image.h"
#include "Vector.h"
#include "TraceableList.h"
#include "Sphere.h"
#include "Ray.h"
#include "Camera.h"
#include "PPMWriter.h"
#include <stdlib.h>


using Real = ztrace::Real;
using Int = ztrace::Int;
using Vector = ztrace::Vector;
using Ray = ztrace::Ray;
using Colour = ztrace::ColourConverter<128>;
using Image = ztrace::Image<Colour>;
using PPMWriter = ztrace::PPMWriter<Image>;
using TraceableList = ztrace::TraceableList;
using TraceData = ztrace::TraceData;
using Sphere = ztrace::Sphere;
using Camera = ztrace::Camera;

Vector const selectRandomInSphere( ){
  Vector result;
  Vector unit{1.,1.,1.};
  do {
      result = 2. * Vector{drand48(), drand48(), drand48()} - unit;
  } while( result.len() > 1. );
  return result;
}

Colour const rayColour( Ray const & ray, TraceableList & traceable  ){
    Colour colour;
    TraceData traceData;
    bool hitSomething = traceable.hit( ray, 1e-2, 1000., traceData );
    if ( hitSomething ){
        colour = 0.5 * rayColour( Ray{ traceData.point, traceData.normal * 0.8 + selectRandomInSphere() }, traceable ).vector();
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

    Int width  = 400;
    Int height = 225;
    Int antiAliasing = 16;

    Image image( width, height );

    TraceableList traceables{};
    traceables.add( std::make_shared<Sphere>(Vector{1.,   1.,-3.5}, 0.25 ));
    traceables.add( std::make_shared<Sphere>(Vector{0.,   1.,-3.5}, 0.75 ));
    traceables.add( std::make_shared<Sphere>(Vector{0.,-101.,-1.5}, 100.5 ));

    Camera cam{};

    for( Int y = 0; y < height; ++y ){
        for( Int x = 0; x < width; ++x ){
            Real xPos, yPos;
            Vector colour;
            for( Int a = 0; a < antiAliasing; ++a ) {
                xPos = position( x, width );
                yPos = position( y, height);
                Ray    ray = cam.emitRay( xPos, yPos );
                colour += rayColour(ray, traceables).vector() / antiAliasing;
            }
            image.setPixel( x, y, colour);
        }
        std::cout << (Real) y / (Real) height * 100. << " %" << std::endl;
    }
    PPMWriter writer;
    writer( "sample.ppm", image );
    return 0;
}
