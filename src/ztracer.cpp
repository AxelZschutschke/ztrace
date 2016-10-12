#include <iostream>
#include <iomanip>
#include <stdlib.h>

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
#include "CreateAmbientLights.h"


using namespace ztrace;
using ImageType = Image<Vector,ColourConverter<255>>;

Vector const lightIntensity( LightList const & lights, TraceableList const & traceable, TraceData const & traceData )
{
    static Real const tolerance = 1e-3;

    Vector lightIntensityAllLights{0.,0.,0.};
    TraceData traceDataShadow;
    for( auto light: lights ) {
        Vector materialAttenuation;
        Vector lightIntensitySingle;
        ShadowRay shadowRay;
        if( light->getShadowRay( traceData.point, lightIntensitySingle, shadowRay ) ) {
            bool hit;
            hit = traceable.hit( shadowRay, tolerance, 1000., traceDataShadow );
            if( hit and ( traceDataShadow.point - traceData.point ).len() > tolerance )
            {
                // we hit something in between, check whether transmissive, repeat until hitting something solid
                ShadowRay retryShadowRay = shadowRay;
                while( hit and 
                        ( traceDataShadow.point - traceData.point ).len() > tolerance and 
                        traceDataShadow.material != nullptr and 
                        traceDataShadow.material->getGloss().hasTransmission() )
                {
                    retryShadowRay.origin() = traceDataShadow.point;
                    lightIntensitySingle *= traceDataShadow.material->getGloss().transmissivity();
                    hit = traceable.hit( retryShadowRay, tolerance, 1000., traceDataShadow );
                }
            }
            if( ( traceDataShadow.point - traceData.point ).len() < tolerance )
            {
                if( traceData.material->scatterLight( shadowRay, traceData, materialAttenuation ) ) {
                    lightIntensityAllLights += lightIntensitySingle * materialAttenuation;
                }
            }
        }
    }
    return lightIntensityAllLights;
}
Vector const rayColour( Ray const & ray, TraceableList & traceable, LightList & lights, Int depth = 0  ){
    static Int const maxDepth = 10;
    static Real const tolerance = 1e-3;

    Vector colour{0.,0.,0.};
    TraceData traceData;
    if ( traceable.hit( ray, tolerance, 100., traceData ) ){
        Vector attenuation;
        Ray scatteredRay;
        if( depth < maxDepth and traceData.material->scatterView( ray, traceData, attenuation, scatteredRay )){
            colour += attenuation * rayColour( scatteredRay, traceable, lights, ++depth );
            colour += lightIntensity( lights, traceable, traceData );
        } // else colour = black (which it is already)
    } else {
        Vector unit_direction = ray.direction();
        unit_direction.makeUnitVector();

        Real t = 0.5 * (unit_direction.y() + 1.0);
        colour = (1.0 - t) * Vector(0.5,0.5,0.6) + t * Vector(0.15, 0.25, 0.5);
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
    Int antiAliasing = 2;
    Int numberDynamicLights = 50; // maximum forward rays per source-light

    ImageType image( width, height );

    Gloss diffusiveLightRed( true, false, false, {0.9,0.1,0.5}, 90. );
    Gloss diffusiveLightGreen( true, false, false, {0.1,0.9,0.5}, 50. );
    Gloss diffusiveLightBlue( true, false, false, {0.1,0.5,0.9}, 1. );
    Gloss specularMetalGrey( true, true, false, {0.4,0.4,0.4}, 100., {0.9,0.9,0.9} );
    Gloss specularWhiteGlass( false, true, true, {0.9,0.9,0.9}, 90., {1.,1.,1.}, {0.7,0.7,0.7});

    TraceableList traceables{};
    traceables.add( std::make_shared<Sphere>(Vector{-0.5,  0.69,-14.0},   0.7, std::make_shared<Metal>(diffusiveLightBlue )  ));
    traceables.add( std::make_shared<Sphere>(Vector{ 1.5,  0.69, -4.0},   0.7, std::make_shared<Metal>(diffusiveLightGreen )  ));
    traceables.add( std::make_shared<Sphere>(Vector{ 0.3,  0.19, -5.5},   0.2, std::make_shared<Metal>(specularMetalGrey )  ));
    traceables.add( std::make_shared<Sphere>(Vector{ 1.8,  0.56,-19.5},   0.6, std::make_shared<Metal>(diffusiveLightBlue )  ));
    traceables.add( std::make_shared<Sphere>(Vector{-2.6,  0.35, -13.1},  0.36, std::make_shared<Lambertian>(diffusiveLightGreen)  ));
    traceables.add( std::make_shared<Sphere>(Vector{-1.1,  0.28, -3.1},   0.3, std::make_shared<Lambertian>(diffusiveLightRed)  ));
    traceables.add( std::make_shared<Sphere>(Vector{ 0.0,  0.24, -5.0},  0.25, std::make_shared<Glass>(specularWhiteGlass)  ));
    traceables.add( std::make_shared<Sphere>(Vector{ 0.0,  0.24, -5.0}, -0.21, std::make_shared<Glass>(specularWhiteGlass)  ));
    traceables.add( std::make_shared<Sphere>(Vector{-1.0,  0.75, -8.5},  0.75, std::make_shared<Glass>(specularWhiteGlass)  ));
    traceables.add( std::make_shared<Sphere>(Vector{-1.0,  0.75, -8.5},  -0.7, std::make_shared<Glass>(specularWhiteGlass)  ));
    traceables.add( std::make_shared<Sphere>(Vector{ 00.,-1001.0,-10.}, 1001.0, std::make_shared<Metal>(diffusiveLightGreen) ));
    
    LightList lights{};
    lights.add( std::make_shared<LightSpot>(Vector{2.,2.,2.},Vector{1.,1.,1.},135.,Vector{-1.0,-0.4,-1.},0.2 ) );
    lights.add( std::make_shared<LightSpot>(Vector{0.0,10.,-5.0},Vector{0.5,0.7,1.},127.,Vector{0.,-1.,0.},0.2,false ) );

    Camera cam({0.,1.0,1.},{0.,-0.1,-1.}, 5.333333333, 3.0, 9.5, 0.025 );

    Vector colour;
    for( Int a = 0; a < antiAliasing; ++a ) {
        Real xPos, yPos;
        LightList dynamicLights = ztrace::createAmbientLights( lights, traceables, numberDynamicLights, 6, 0.05 );
        std::cout << "Number of lights: " << dynamicLights.size() << std::endl;
        for( Int y = 0; y < height; ++y ){
            for( Int x = 0; x < width; ++x ){
                colour = image.getPixel( x, y );
                xPos = jitter( x, width );
                yPos = jitter( y, height);
                Ray    ray = cam.emitRay( xPos, yPos );
                colour += rayColour(ray, traceables, dynamicLights ) / (Real) antiAliasing;
                image.setPixel( x, y, colour);
            }
        }
        std::cout << std::setprecision(3) << (Real) ( a + 1 ) / (Real) antiAliasing * 100. << " %" << std::endl;
    }

    PPMWriter<ImageType> writer;
    writer( "sample.ppm", image );
    return 0;
}
