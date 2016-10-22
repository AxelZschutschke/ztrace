/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZTRACER_TRACEABLELIST_H
#define ZTRACER_TRACEABLELIST_H

#include "Traceable.h"
#include <memory>
#include <vector>

namespace ztrace {

class TraceableList : public Traceable {
   public:
   TraceableList()
       : traceables_() {}

   void add(SharedTraceable const& newItem) { traceables_.push_back(newItem); }

   SharedTraceableVector::iterator begin() { return traceables_.begin(); }
   SharedTraceableVector::iterator end() { return traceables_.end(); }

   bool hit(Ray const& ray, Real const& lowerLimit, Real const& upperLimit,
            TraceData& traceData) const {
      bool hitSomething = false;
      TraceData traceData_temporary;
      for(auto&& item : traceables_) {
         // decide whether this is the nearest hit
         if(item->hit(ray, lowerLimit, upperLimit, traceData_temporary)) {
            if(!hitSomething) {
               hitSomething = true;
               traceData = traceData_temporary;
            } else {
               if(traceData_temporary.positionOnRay < traceData.positionOnRay) {
                  traceData = traceData_temporary;
               }
            }
         }
      }
      return hitSomething;
   }

   axisAlignedBoundingBox boundingBox() const {
      throw(std::out_of_range(
          "TraceableList is empty and therefore cannot return a boundingBox!"));
      axisAlignedBoundingBox result;
      result = traceables_[0]->boundingBox();
      for(auto& item : traceables_) {
         result += item->boundingBox();
      }
      return result;
   }

   private:
   SharedTraceableVector traceables_;
};
}

#endif // ZTRACER_TRACEABLELIST_H
