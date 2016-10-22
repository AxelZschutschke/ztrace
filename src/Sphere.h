/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZTRACE_SPHERE_H
#define ZTRACE_SPHERE_H

#include <cmath> // sqrt

#include "Traceable.h"
#include "Types.h"
#include "Utils.h"

namespace ztrace {
class Sphere : public Traceable {
   using MaterialPtr = std::shared_ptr<Material>;

   public:
   Sphere() = default;

   Sphere(Vector const& center, Real const& radius, MaterialPtr material = nullptr)
       : center_(center)
       , radius_(radius)
       , material_(material) {}

   static bool checkHitInterval(Real const& positionOnRay, Real const& intervalLower,
                                Real const& intervalUpper) {
      if(positionOnRay < intervalLower or positionOnRay > intervalUpper) {
         return false;
      }
      return true;
   }
   bool hit(Ray const& ray, Real const& intervalLower, Real const& intervalUpper,
            TraceData& traceData) const {
      Vector oc = ray.origin() - center_;
      Real a = dot(ray.direction(), ray.direction());
      Real b = 2.0 * dot(oc, ray.direction());
      Real c = dot(oc, oc) - radius_ * radius_;
      Real discriminant = (b * b - 4. * a * c);
      if(discriminant <= 0.) {
         return false;
      }

      Real positionOnRay = (-b - sqrt(discriminant)) / (2.0 * a);
      if(!checkHitInterval(positionOnRay, intervalLower, intervalUpper)) {
         return false;
      }

      traceData.positionOnRay = positionOnRay;
      traceData.point = ray.positionAtLength(traceData.positionOnRay);
      traceData.normal = traceData.point - center_;
      traceData.normal.makeUnitVector();
      traceData.reflection = reflect(ray.direction(), traceData.normal);
      traceData.reflection.makeUnitVector();
      traceData.material = material_;
      return true;
   }
   axisAlignedBoundingBox boundingBox() const {
      return axisAlignedBoundingBox{center_ - Vector{radius_, radius_, radius_},
                 center_ + Vector{radius_, radius_, radius_}};
   }

   Vector const& center() const { return center_; }

   Real const& radius() const { return radius_; }

   private:
   Vector center_;
   Real radius_;
   MaterialPtr material_;
};
}

#endif // ZTRACE_SPHERE_H
