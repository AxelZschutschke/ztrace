//
// Created by Axel Zschutschke on 10/9/16.
//

#ifndef ZTRACER_TRACEABLE_H
#define ZTRACER_TRACEABLE_H

#include "axisAlignedBoundingBox.h"
#include "Ray.h"
#include <vector>

namespace ztrace {

class Material;

struct TraceData {
   using MaterialPtr = std::shared_ptr<Material>;
   Real positionOnRay;
   Vector point;
   Vector normal;
   Vector reflection;
   MaterialPtr material;
};

class Traceable {
   public:
   using SharedTraceable = std::shared_ptr<Traceable>;
   using SharedTraceableVector = std::vector<SharedTraceable>;

   // simple check: determine if hit is in correct interval and get data of object hit
   virtual bool hit(Ray const& ray, Real const& intervalLower, Real const& intervalUpper,
                    TraceData& traceData) const = 0;
   // create bounding box object of respective
   virtual axisAlignedBoundingBox boundingBox() const = 0;
};
}

#endif // ZTRACER_TRACEABLE_H
