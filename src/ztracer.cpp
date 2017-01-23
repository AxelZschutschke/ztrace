/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <atomic>
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
    static Real const tolerance = (Real) 1e-3;

    std::atomic<Vector> lightIntensityAllLights(Vector{0.,0.,0.});
    for_each( lights.begin(), lights.end(), [&]( std::shared_ptr<Light> const & light ){
        TraceData traceDataShadow;
        Vector materialAttenuation;
        Vector lightIntensitySingle;
        ShadowRay shadowRay;
        if( light->getShadowRay( traceData.point, lightIntensitySingle, shadowRay ) ) {
            bool hit;
            hit = traceable.hit( shadowRay, tolerance, 1000., traceDataShadow );
            if( hit && ( traceDataShadow.point - traceData.point ).len() > tolerance )
            {
                // we hit something in between, check whether transmissive, repeat until hitting something solid
                ShadowRay retryShadowRay = shadowRay;
                while( hit &&
                        ( traceDataShadow.point - traceData.point ).len() > tolerance &&
                        traceDataShadow.material != nullptr &&
                        traceDataShadow.material->gloss().hasTransmission() )
                {
                    retryShadowRay.origin() = traceDataShadow.point;
                    lightIntensitySingle *= traceDataShadow.material->gloss().transmissivity();
                    hit = traceable.hit( retryShadowRay, tolerance, 1000., traceDataShadow );
                }
            }
            if( ( traceDataShadow.point - traceData.point ).len() < tolerance )
            {
                if( traceData.material->scatterLight( shadowRay, traceData, materialAttenuation ) ) {
                    lightIntensityAllLights = lightIntensityAllLights.load() + lightIntensitySingle * materialAttenuation;
                }
            }
        }
    });
    return lightIntensityAllLights;
}
Vector const rayColour( Ray const & ray, TraceableList & traceable, LightList & lights, Int depth = 0  ){
    static Int const maxDepth = 10;
    static Real const tolerance = (Real) 1e-3;

    Vector colour{0.,0.,0.};
    TraceData traceData;
    if ( traceable.hit( ray, tolerance, 100., traceData ) ){
        if( depth < 2 ) {
            // first time we hit something, we get two rays back -> faster convergence for mirror/glass
            Vector attenuation1;
            Ray scatteredRay1;
            Vector attenuation2;
            Ray scatteredRay2;
            Real weight1;
            if( traceData.material->scatterView( ray, traceData, attenuation1, scatteredRay1, attenuation2, scatteredRay2, weight1 )){
                colour += weight1 * attenuation1 * rayColour( scatteredRay1, traceable, lights, depth+1 );
                colour += ((Real) 1. - weight1 ) * attenuation2 * rayColour( scatteredRay2, traceable, lights, depth+1 );
                colour += lightIntensity( lights, traceable, traceData );
            }
        } else if( depth < maxDepth ) {
            Vector attenuation;
            Ray scatteredRay;
            if( traceData.material->scatterView( ray, traceData, attenuation, scatteredRay )){
                colour += attenuation * rayColour( scatteredRay, traceable, lights, ++depth );
                colour += lightIntensity( lights, traceable, traceData );
            }
        } // else colour = black (which it is already)
    } else {
        Vector unit_direction = ray.direction();
        unit_direction.makeUnitVector();

        Real t = (Real) 0.5 * (unit_direction.y() + (Real) 1.0);
        colour = ((Real) 1.0 - t) * Vector((Real)0.8,(Real)0.8,(Real)1.0) + t * Vector((Real)0.15,(Real)0.25,(Real)0.5);
    }
    return colour;
}

Real jitter( Int const & x, Int const & screenWidth ) {
    return ((Real) x + (ztrace::random() - (Real)0.5) ) / ((Real) screenWidth);
}

void writeSample( String const & basename, 
        Real const & progress, 
        ImageType const & instantImageData,
        String extension = ".ppm" ) 
{
    ImageType sample( instantImageData.width(), instantImageData.height() );
    std::transform( instantImageData.begin(), instantImageData.end(), sample.begin(), 
            [=]( Vector const & instantValue ){ return instantValue / progress; } );

    PPMWriter<ImageType> writer;
    writer( basename + std::to_string(progress) + extension, sample );
}

int main()
{
    randomInit( 185922891234523452 );

    String basename = "raySplit";
    String extension = ".ppm";

    //Int width  = 150;
    //Int height = 100;
    Int width  = 1778;
    Int height = 1000;
    Int antiAliasing = 4;
    Int numberDynamicLights = 20; // maximum forward rays per source-light


    Gloss diffusiveLightRed( true, false, false, {(Real)0.9,(Real)0.1,(Real)0.5}, (Real)90. );
    Gloss diffusiveLightGreen( true, false, false, {(Real)0.1,(Real)0.9,(Real)0.5}, (Real)50. );
    Gloss diffusiveLightBlue( true, false, false, { (Real)0.1,(Real)0.5,(Real)0.9}, (Real)1. );
    Gloss specularMetalGrey( true, true, false, { (Real)0.4,(Real)0.4,(Real)0.4}, (Real)100., { (Real)0.9,(Real)0.9,(Real)0.9} );
    Gloss specularWhiteGlass( false, true, true, { (Real)0.9,(Real)0.9,(Real)0.9}, (Real)100., { (Real)1.,(Real)1.,(Real)1.}, { (Real)0.7,(Real)0.7,(Real)0.7});

    TraceableList traceables{};
    traceables.add( std::make_shared<Sphere>(Vector{ (Real)-0.5, (Real)0.69,(Real)-14.0},   (Real)0.7, std::make_shared<Metal>(diffusiveLightBlue )  ));
    traceables.add( std::make_shared<Sphere>(Vector{ (Real)1.5, (Real)0.69, (Real)-4.0}, (Real)0.7, std::make_shared<Metal>(diffusiveLightGreen )  ));
    traceables.add( std::make_shared<Sphere>(Vector{ (Real)0.3, (Real)0.19, (Real)-5.5}, (Real)0.2, std::make_shared<Metal>(specularMetalGrey )  ));
    traceables.add( std::make_shared<Sphere>(Vector{ (Real)1.8, (Real)0.56,(Real)-19.5}, (Real)0.6, std::make_shared<Metal>(diffusiveLightBlue )  ));
    traceables.add( std::make_shared<Sphere>(Vector{ (Real)-2.6, (Real)0.35, (Real)-13.1}, (Real)0.36, std::make_shared<Lambertian>(diffusiveLightGreen)  ));
    traceables.add( std::make_shared<Sphere>(Vector{ (Real)-1.1,  (Real)0.28, (Real)-3.1}, (Real)0.3, std::make_shared<Lambertian>(diffusiveLightRed)  ));
    traceables.add( std::make_shared<Sphere>(Vector{ (Real)0.0,  (Real)0.24, (Real)-5.0}, (Real)0.25, std::make_shared<Glass>(specularWhiteGlass)  ));
    traceables.add( std::make_shared<Sphere>(Vector{ (Real)0.0,  (Real)0.24, (Real)-5.0}, (Real)-0.21, std::make_shared<Glass>(specularWhiteGlass)  ));
    traceables.add( std::make_shared<Sphere>(Vector{ (Real)-1.0,  (Real)0.75, (Real)-8.5}, (Real)0.75, std::make_shared<Glass>(specularWhiteGlass)  ));
    traceables.add( std::make_shared<Sphere>(Vector{ (Real)-1.0, (Real)0.75, (Real)-8.5}, (Real)-0.7, std::make_shared<Glass>(specularWhiteGlass)  ));
    traceables.add( std::make_shared<Sphere>(Vector{ (Real)00.,(Real)-1001.0,(Real)-10.}, (Real)1001.0, std::make_shared<Metal>(diffusiveLightGreen) ));
    
    LightList lightsReceivingDynamicPath{};
    lightsReceivingDynamicPath.add( std::make_shared<LightSpot>(Vector{ (Real)2.,(Real)2.,(Real)2.},Vector{ (Real)1.,(Real)1.,(Real)1.}, (Real)15.,Vector{ (Real)-0.5,(Real)-0.4,(Real)-2.}, (Real)0.2 ) );
    LightList lightsNotReceivingDynamicPath{};
    lightsNotReceivingDynamicPath.add( std::make_shared<LightSpot>(Vector{ (Real)0.0,(Real)30.,(Real)5.0},Vector{ (Real)0.5,(Real)0.7,(Real)1.}, (Real)27.,Vector{ (Real)0.,(Real)-1.,(Real)-0.5}, (Real)0.4,false ) );

    Camera cam({ (Real)0.,(Real)1.0,(Real)1.},{ (Real)0.,(Real)-0.1,(Real)-1.}, (Real)5.333333333, (Real)3.0, (Real)9.5, (Real)0.025 );

    Vector colour;
    ImageType image( width, height );

    for( Int a = 0; a < antiAliasing; ++a ) {
        Real xPos, yPos;
        LightList dynamicLights = ztrace::createAmbientLights( lightsReceivingDynamicPath, traceables, numberDynamicLights, 6, (Real)0.05 );
        dynamicLights.add( lightsNotReceivingDynamicPath );
        std::cout << "Number of lights: " << dynamicLights.size() << std::endl;
        for( Int y = 0; y < height; ++y ){
            for( Int x = 0; x < width; ++x ){
                colour = image.getPixel( x, y );
                xPos = jitter( x, width );
                yPos = jitter( y, height);
                Ray    ray1;
                Ray    ray2;
                cam.emitRayPair( xPos, yPos, ray1, ray2 );
                colour += 0.5 * rayColour(ray1, traceables, dynamicLights ) / (Real) antiAliasing;
                colour += 0.5 * rayColour(ray2, traceables, dynamicLights ) / (Real) antiAliasing;
                image.setPixel( x, y, colour);
            }
        }
        Real progress = (Real) ( a + 1 ) / (Real) antiAliasing;
        writeSample( basename, progress, image );
        std::cout << std::setprecision(3) << progress * 100. << " %" << std::endl;
    }

    PPMWriter<ImageType> writer;
    writer( basename + extension, image );
    return 0;
}
