#include <iomanip>
#include <iostream>

#include "Random.h"
#include "Camera.h"
#include "ColourConverter.h"
#include "CreateAmbientLights.h"
#include "Image.h"
#include "Light.h"
#include "LightList.h"
#include "Material.h"
#include "PPMWriter.h"
#include "Ray.h"
#include "Sphere.h"
#include "TraceableList.h"
#include "TraceableNode.h"
#include "Vector.h"


using namespace ztrace;
using ImageType = Image<Vector, ColourConverter<255>>;

Vector const lightIntensity(LightList const& lights, Traceable const& traceable,
                            TraceData const& traceData) {
   static Real const tolerance = 1e-3;

   Vector lightIntensityAllLights{0., 0., 0.};
   TraceData traceDataShadow;
   for(auto light : lights) {
      Vector materialAttenuation;
      Vector lightIntensitySingle;
      ShadowRay shadowRay;
      if(light->getShadowRay(traceData.point, lightIntensitySingle, shadowRay)) {
         bool hit;
         hit = traceable.hit(shadowRay, tolerance, 1000., traceDataShadow);
         if(hit and (traceDataShadow.point - traceData.point).len() > tolerance) {
            // we hit something in between, check whether transmissive, repeat until
            // hitting something solid
            ShadowRay retryShadowRay = shadowRay;
            while(hit and (traceDataShadow.point - traceData.point).len() > tolerance
                  and traceDataShadow.material != nullptr
                  and traceDataShadow.material->gloss().hasTransmission()) {
               retryShadowRay.origin() = traceDataShadow.point;
               lightIntensitySingle *=
                   traceDataShadow.material->gloss().transmissivity();
               hit = traceable.hit(retryShadowRay, tolerance, 1000., traceDataShadow);
            }
         }
         if((traceDataShadow.point - traceData.point).len() < tolerance) {
            if(traceData.material->scatterLight(shadowRay, traceData,
                                                materialAttenuation)) {
               lightIntensityAllLights += lightIntensitySingle * materialAttenuation;
            }
         }
      }
   }
   return lightIntensityAllLights;
}
Vector const rayColour(Ray const& ray, Traceable& traceable, LightList& lights,
                       Int depth = 0) {
   static Int const maxDepth = 10;
   static Real const tolerance = 1e-3;

   Vector colour{0., 0., 0.};
   TraceData traceData;
   if(traceable.hit(ray, tolerance, 100., traceData)) {
      if(depth < 2) {
         // first time we hit something, we get two rays back -> faster convergence for
         // mirror/glass
         Vector attenuation1;
         Ray scatteredRay1;
         Vector attenuation2;
         Ray scatteredRay2;
         Real weight1;
         if(traceData.material->scatterView(ray, traceData, attenuation1, scatteredRay1,
                                            attenuation2, scatteredRay2, weight1)) {
            colour += weight1 * attenuation1
                      * rayColour(scatteredRay1, traceable, lights, depth + 1);
            colour += (1. - weight1) * attenuation2
                      * rayColour(scatteredRay2, traceable, lights, depth + 1);
            colour += lightIntensity(lights, traceable, traceData);
         }
      } else if(depth < maxDepth) {
         Vector attenuation;
         Ray scatteredRay;
         if(traceData.material->scatterView(ray, traceData, attenuation, scatteredRay)) {
            colour += attenuation * rayColour(scatteredRay, traceable, lights, ++depth);
            colour += lightIntensity(lights, traceable, traceData);
         }
      } // else colour = black (which it is already)
   } else {
      Vector unit_direction = ray.direction();
      unit_direction.makeUnitVector();

      Real t = 0.5 * (unit_direction.y() + 1.0);
      colour = (1.0 - t) * Vector(0.8, 0.8, 1.0) + t * Vector(0.15, 0.25, 0.5);
   }
   return colour;
}

Real jitter(Int const& x, Int const& screenWidth) {
   return ((Real) x + (ztrace::random() - 0.5)) / ((Real) screenWidth);
}

void writeSample(String const& basename, Real const& progress,
                 ImageType const& instantImageData, String extension = ".ppm") {
   ImageType sample(instantImageData.width(), instantImageData.height());
   std::transform(instantImageData.begin(), instantImageData.end(), sample.begin(),
                  [=](Vector const& instantValue) { return instantValue / progress; });

   PPMWriter<ImageType> writer;
   writer(basename + std::to_string(progress) + extension, sample);
}

int main() {
   srand48(185922891234523452);

   String basename = "raySplit";
   String extension = ".ppm";

   Int width = 711;
   Int height = 400;
   // Int width  = 1778;
   // Int height = 1000;
   Int antiAliasing = 1;
   Int numberDynamicLights = 100; // maximum forward rays per source-light


   Gloss diffusiveLightRed(true, false, false, {0.9, 0.1, 0.5}, 90.);
   Gloss diffusiveLightGreen(true, false, false, {0.1, 0.9, 0.5}, 50.);
   Gloss diffusiveLightBlue(true, false, false, {0.1, 0.5, 0.9}, 1.);
   Gloss specularMetalGrey(true, true, false, {0.4, 0.4, 0.4}, 100., {0.9, 0.9, 0.9});
   Gloss specularWhiteGlass(false, true, true, {0.9, 0.9, 0.9}, 100., {1., 1., 1.},
                            {0.7, 0.7, 0.7});

   Traceable::SharedTraceableVector unsortedTraceables{};
   unsortedTraceables.push_back(std::make_shared<Sphere>(
       Vector{-0.5, 0.69, -14.0}, 0.7, std::make_shared<Metal>(diffusiveLightBlue)));
   unsortedTraceables.push_back(std::make_shared<Sphere>(
       Vector{1.5, 0.69, -4.0}, 0.7, std::make_shared<Metal>(diffusiveLightGreen)));
   unsortedTraceables.push_back(std::make_shared<Sphere>(
       Vector{0.3, 0.19, -5.5}, 0.2, std::make_shared<Metal>(specularMetalGrey)));
   unsortedTraceables.push_back(std::make_shared<Sphere>(
       Vector{1.8, 0.56, -19.5}, 0.6, std::make_shared<Metal>(diffusiveLightBlue)));
   unsortedTraceables.push_back(
       std::make_shared<Sphere>(Vector{-2.6, 0.35, -13.1}, 0.36,
                                std::make_shared<Lambertian>(diffusiveLightGreen)));
   unsortedTraceables.push_back(std::make_shared<Sphere>(
       Vector{-1.1, 0.28, -3.1}, 0.3, std::make_shared<Lambertian>(diffusiveLightRed)));
   unsortedTraceables.push_back(std::make_shared<Sphere>(
       Vector{0.0, 0.24, -5.0}, 0.25, std::make_shared<Glass>(specularWhiteGlass)));
   unsortedTraceables.push_back(std::make_shared<Sphere>(
       Vector{0.0, 0.24, -5.0}, -0.21, std::make_shared<Glass>(specularWhiteGlass)));
   unsortedTraceables.push_back(std::make_shared<Sphere>(
       Vector{-1.0, 0.75, -8.5}, 0.75, std::make_shared<Glass>(specularWhiteGlass)));
   unsortedTraceables.push_back(std::make_shared<Sphere>(
       Vector{-1.0, 0.75, -8.5}, -0.7, std::make_shared<Glass>(specularWhiteGlass)));
   unsortedTraceables.push_back(std::make_shared<Sphere>(
       Vector{0.0, -1001.0, -10.}, 1001.0, std::make_shared<Metal>(diffusiveLightGreen)));
   TraceableNode traceables(unsortedTraceables);

   LightList lightsReceivingDynamicPath{};
   lightsReceivingDynamicPath.add(std::make_shared<LightSpot>(
       Vector{2., 2., 2.}, Vector{1., 1., 1.}, 15., Vector{-0.5, -0.4, -2.}, 0.2));
   LightList lightsNotReceivingDynamicPath{};
   lightsNotReceivingDynamicPath.add(
       std::make_shared<LightSpot>(Vector{0.0, 30., 5.0}, Vector{0.5, 0.7, 1.}, 27.,
                                   Vector{0., -1., -0.5}, 0.4, false));

   Camera cam({0., 1.0, 1.}, {0., -0.1, -1.}, 5.333333333, 3.0, 9.5, 0.025);

   Vector colour;
   ImageType image(width, height);

   for(Int a = 0; a < antiAliasing; ++a) {
      Real xPos, yPos;
      LightList dynamicLights = ztrace::createAmbientLights(
          lightsReceivingDynamicPath, traceables, numberDynamicLights, 6, 0.05);
      dynamicLights.add(lightsNotReceivingDynamicPath);
      std::cout << "Number of lights: " << dynamicLights.size() << std::endl;
      for(Int y = 0; y < height; ++y) {
         for(Int x = 0; x < width; ++x) {
            colour = image.getPixel(x, y);
            xPos = jitter(x, width);
            yPos = jitter(y, height);
            Ray ray1;
            Ray ray2;
            cam.emitRayPair(xPos, yPos, ray1, ray2);
            colour +=
                0.5 * rayColour(ray1, traceables, dynamicLights) / (Real) antiAliasing;
            colour +=
                0.5 * rayColour(ray2, traceables, dynamicLights) / (Real) antiAliasing;
            image.setPixel(x, y, colour);
         }
      }
      Real progress = (Real)(a + 1) / (Real) antiAliasing;
      writeSample(basename, progress, image);
      std::cout << std::setprecision(3) << progress * 100. << " %" << std::endl;
   }

   PPMWriter<ImageType> writer;
   writer(basename + extension, image);
   return 0;
}
