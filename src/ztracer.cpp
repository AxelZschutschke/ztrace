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
            traceable.hit( shadowRay, 1.e-3, 1000., traceDataShadow );
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
    static Int const maxDepth = 40;

    Vector colour{0.,0.,0.};
    TraceData traceData;
    if ( traceable.hit( ray, 1e-3, 1000., traceData ) ){
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
        //colour = (1.0 - t) * Vector(1., 1., 1.) + t * Vector(0.5, 0.7, 1.0);
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
    Int antiAliasing = 4;

    ImageType image( width, height );

    Gloss diffusiveLightRed( true, false, false, {0.9,0.1,0.5} );
    Gloss diffusiveLightGreen( true, false, false, {0.1,0.9,0.5} );
    Gloss diffusiveLightBlue( true, false, false, {0.1,0.5,0.9} );
    Gloss specularMetalGrey( true, true, false, {0.4,0.4,0.4}, {1.,1.,1.}, 100. );
    Gloss specularWhiteGlass( false, true, true, {0.9,0.9,0.9}, {1.,1.,1.}, 200., {0.9,0.9,0.9});

    TraceableList traceables{};
    traceables.add( std::make_shared<Sphere>(Vector{-0.5,  0.69,-14.0},   0.7, std::make_shared<Metal>(diffusiveLightBlue, 0.1 )  ));
    traceables.add( std::make_shared<Sphere>(Vector{ 1.5,  0.69, -4.0},   0.7, std::make_shared<Metal>(diffusiveLightGreen, 0.4 )  ));
    traceables.add( std::make_shared<Sphere>(Vector{ 0.3,  0.19, -5.5},   0.2, std::make_shared<Metal>(specularMetalGrey, 0.0 )  ));
    traceables.add( std::make_shared<Sphere>(Vector{ 1.8,  0.56,-19.5},   0.6, std::make_shared<Metal>(diffusiveLightBlue, 0.0 )  ));
    traceables.add( std::make_shared<Sphere>(Vector{-2.6,  0.35, -13.1},  0.36, std::make_shared<Lambertian>(diffusiveLightGreen)  ));
    traceables.add( std::make_shared<Sphere>(Vector{-1.1,  0.28, -3.1},   0.3, std::make_shared<Lambertian>(diffusiveLightRed)  ));
    traceables.add( std::make_shared<Sphere>(Vector{ 0.0,  0.24, -5.0},  0.25, std::make_shared<Glass>(specularWhiteGlass, 1.5 )  ));
    traceables.add( std::make_shared<Sphere>(Vector{ 0.0,  0.24, -5.0}, -0.21, std::make_shared<Glass>(specularWhiteGlass, 1.5 )  ));
    traceables.add( std::make_shared<Sphere>(Vector{-1.0,  0.74, -8.5},  0.75, std::make_shared<Glass>(specularWhiteGlass, 1.5 )  ));
    traceables.add( std::make_shared<Sphere>(Vector{-1.0,  0.74, -8.5},  -0.7, std::make_shared<Glass>(specularWhiteGlass, 1.5 )  ));
    traceables.add( std::make_shared<Sphere>(Vector{ 00.,-1001.0,-10.}, 1001.0, std::make_shared<Metal>(diffusiveLightGreen, 0.6) ));
    
    LightList lights{};
    lights.add( std::make_shared<LightSpot>(Vector{2.,2.,2.},Vector{1.,1.,1.},40.,Vector{-0.9,-0.2,-2.},0.05 ) );
    //lights.add( std::make_shared<LightSpot>(Vector{0.,22.,0.},Vector{1.,1.,1.},22.,Vector{0.,-1.,0.},0.8,false ) );

    Camera cam({0.,1.0,1.},{0.,-0.1,-1.}, 5.333333333, 3.0, 9.5, 0.025 );

    Vector colour;
    for( Int a = 0; a < antiAliasing; ++a ) {
        Real xPos, yPos;
        LightList dynamicLights = ztrace::createAmbientLights( lights, traceables, 100, 4, 0.05 );
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
