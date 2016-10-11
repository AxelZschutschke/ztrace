#include <iostream>
#include "ColourConverter.h"
#include "Image.h"
#include "Vector.h"
#include "TraceableList.h"
#include "LightList.h"
#include "Light.h"
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
using Image = ztrace::Image<ztrace::Vector,ztrace::ColourConverter<255>>;
using PPMWriter = ztrace::PPMWriter<Image>;
using TraceableList = ztrace::TraceableList;
using TraceData = ztrace::TraceData;
using Sphere = ztrace::Sphere;
using Camera = ztrace::Camera;
using Lambertian = ztrace::Lambertian;
using Metal = ztrace::Metal;
using Glass = ztrace::Glass;
using PointLight = ztrace::LightPointSource;
using LightList = ztrace::LightList;
using Light = ztrace::Light;
using LightPointSource = ztrace::LightPointSource;


Vector const lightIntensity( Ray const & rayIn, LightList const & lights, TraceableList const & traceable, TraceData const & traceData )
{
    static Real const tolerance = 1e-3;

    Vector lightIntensityAllLights{0.,0.,0.};
    TraceData traceDataShadow;
    for( auto light: lights ) {
        Vector materialAttenuation;
        Vector lightIntensitySingle;
        Ray    shadowRay;
        if( light->getShadowRay( traceData.point, lightIntensitySingle, shadowRay ) ) {
            traceable.hit( shadowRay, 1.e-3, 1000., traceDataShadow );
            if( ( traceDataShadow.point - traceData.point ).len() < tolerance )
            {
                if( traceData.material->scatterLight( rayIn, shadowRay, traceData, materialAttenuation ) ) {
                    lightIntensityAllLights += lightIntensitySingle * materialAttenuation;
                }
            }
        }
    }
    return lightIntensityAllLights;
}
Vector const rayColour( Ray const & ray, TraceableList & traceable, LightList & lights, Int depth = 0  ){
    static Int const maxDepth = 40;

    Vector colour{0.,0.,0.};
    TraceData traceData;
    if ( traceable.hit( ray, 1e-3, 1000., traceData ) ){
        Vector attenuation;
        Ray scatteredRay;
        if( depth < maxDepth and traceData.material->scatterView( ray, traceData, attenuation, scatteredRay )){
            colour += attenuation * rayColour( scatteredRay, traceable, lights, ++depth );
            colour += lightIntensity( ray, lights, traceable, traceData );
        } // else colour = black (which it is already)
    } else {
        Vector unit_direction = ray.direction();
        unit_direction.makeUnitVector();

        Real t = 0.5 * (unit_direction.y() + 1.0);
        colour = (1.0 - t) * Vector(1., 1., 1.) + t * Vector(0.5, 0.7, 1.0);
    }
    return colour;
}

Real jitter( Int const & x, Int const & screenWidth ) {
    return ((Real) x + (drand48() - 0.5) ) / ((Real) screenWidth);
}

int main()
{
    srand48(  185922891234523452 );

    //Int width  = 1778;
    //Int height = 1000;
    //Int antiAliasing = 20;

    Int width  = 711;
    Int height = 400;
    Int antiAliasing = 20;

    Image image( width, height );

    TraceableList traceables{};
    traceables.add( std::make_shared<Sphere>(Vector{-0.5,  0.69,-14.0},   0.7, std::make_shared<Metal>(Vector{0.2,0.2,0.8}, 0.1 )  ));
    traceables.add( std::make_shared<Sphere>(Vector{ 1.5,  0.69, -4.0},   0.7, std::make_shared<Metal>(Vector{0.4,0.6,0.0}, 0.4 )  ));
    traceables.add( std::make_shared<Sphere>(Vector{ 0.3,  0.19, -5.5},   0.2, std::make_shared<Metal>(Vector{0.8,0.8,0.8}, 0.0 )  ));
    traceables.add( std::make_shared<Sphere>(Vector{ 0.0,  0.24, -5.0},  0.25, std::make_shared<Glass>(Vector{0.99,0.9,0.9}, 1.5 )  ));
    traceables.add( std::make_shared<Sphere>(Vector{ 0.0,  0.24, -5.0}, -0.21, std::make_shared<Glass>(Vector{0.99,0.9,0.9}, 1.5 )  ));
    traceables.add( std::make_shared<Sphere>(Vector{-2.6,  0.35, -13.1},  0.36, std::make_shared<Lambertian>(Vector{0.5,0.5,0.0})  ));
    traceables.add( std::make_shared<Sphere>(Vector{-1.1,  0.28, -3.1},   0.3, std::make_shared<Lambertian>(Vector{0.0,0.6,0.6})  ));
    traceables.add( std::make_shared<Sphere>(Vector{-1.0,  0.74, -8.5},  0.75, std::make_shared<Glass>(Vector{0.99,0.9,0.9}, 1.5 )  ));
    traceables.add( std::make_shared<Sphere>(Vector{-1.0,  0.74, -8.5},  -0.7, std::make_shared<Glass>(Vector{0.99,0.9,0.9}, 1.5 )  ));
    traceables.add( std::make_shared<Sphere>(Vector{ 1.8,  0.56,-19.5},   0.6, std::make_shared<Metal>(Vector{0.8,0.8,0.8}, 0.0 )  ));
    traceables.add( std::make_shared<Sphere>(Vector{ 00.,-1001.0,-10.}, 1001.0, std::make_shared<Metal>(Vector{0.1,0.9,0.5}, 0.6) ));
    
    LightList lights{};
    lights.add( std::make_shared<LightPointSource>(Vector{2.,2.,2.},Vector{1.,1.,1.}, 20. ) );

    Camera cam({0.,1.0,1.},{0.,-0.1,-1.}, 5.333333333, 3.0, 9.5, 0.025 );

    Vector colour;
    for( Int y = 0; y < height; ++y ){
        for( Int x = 0; x < width; ++x ){
            colour *= 0.;
            Real xPos, yPos;
            for( Int a = 0; a < antiAliasing; ++a ) {
                xPos = jitter( x, width );
                yPos = jitter( y, height);
                Ray    ray = cam.emitRay( xPos, yPos );
                colour += rayColour(ray, traceables, lights) / (Real) antiAliasing;
            }
            image.setPixel( x, y, colour);
        }
        std::cout << (Real) y / (Real) height * 100. << " %" << std::endl;
    }

    PPMWriter writer;
    writer( "sample.ppm", image );
    return 0;
}
