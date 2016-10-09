#include <iostream>
#include "ColourConverter.h"
#include "Image.h"
#include "Vector.h"
#include "TraceableList.h"
#include "Sphere.h"
#include "Ray.h"
#include "Camera.h"
#include "Material.h"
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
using Lambertian = ztrace::Lambertian;
using Metal = ztrace::Metal;


Colour const rayColour( Ray const & ray, TraceableList & traceable, Int depth = 0  ){
    static Int const maxDepth = 40;

    Colour colour;
    TraceData traceData;
    if ( traceable.hit( ray, 1e-2, 1000., traceData ) ){
        Vector attenuation;
        Ray scatteredRay;
        if( depth < maxDepth && traceData.material->scatter( ray, traceData, attenuation, scatteredRay )) {
            colour = attenuation * rayColour( scatteredRay, traceable, ++depth ).vector();
        } else {
            colour = Vector{0.,0.,0.};
        }
    } else {
        Vector unit_direction = ray.direction();
        unit_direction.makeUnitVector();

        Real t = 0.5 * (unit_direction.y() + 1.0);
        Vector result = (1.0 - t) * Vector(1., 1., 1.) + t * Vector(0.5, 0.7, 1.0);
        colour.setRGB(result.x(), result.y(), result.z());
    }
    return colour;
}

Real position( Int const & x, Int const & screenWidth ) {
    return ((Real) x + (drand48() - 0.5) ) / ((Real) screenWidth);
}

int main()
{

    Int width  = 400;
    Int height = 225;
    Int antiAliasing = 16;

    Image image( width, height );

    TraceableList traceables{};
    traceables.add( std::make_shared<Sphere>(Vector{1.,   1.,-6.5}, 0.5, std::make_shared<Metal>(Vector{0.8,0.8,0.8}, 0.0 )  ));
    traceables.add( std::make_shared<Sphere>(Vector{0.,   1.,-6.5}, 0.75, std::make_shared<Metal>(Vector{0.8,0.1,0.12}, 0.2 )  ));
    traceables.add( std::make_shared<Sphere>(Vector{0.,-101.,-10.}, 100.5, std::make_shared<Metal>(Vector{0.1,0.8,0.12}, 0.5) ));

    Camera cam({0.,0.,0.},{0.,0.,-1.},1.,.5625);

    for( Int y = 0; y < height; ++y ){
        for( Int x = 0; x < width; ++x ){
            Real xPos, yPos;
            Vector colour;
            for( Int a = 0; a < antiAliasing; ++a ) {
                xPos = position( x, width );
                yPos = position( y, height);
                Ray    ray = cam.emitRay( xPos, yPos );
                colour += rayColour(ray, traceables).vector() / (Real) antiAliasing;
            }
            image.setPixel( x, y, colour);
        }
        std::cout << (Real) y / (Real) height * 100. << " %" << std::endl;
    }
    PPMWriter writer;
    writer( "sample.ppm", image );
    return 0;
}
