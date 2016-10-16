#ifndef ZTRACE_TRACEABLE_NODE_H
#define ZTRACE_TRACEABLE_NODE_H

#include "AABB.h"
#include "Traceable.h"

namespace ztrace {

class TraceableNode : public Traceable {
   class Comparator {
      public:
      Comparator(Int axis)
          : axis_(axis) {}

      Int operator()(Traceable::SharedTraceable const& left,
                     Traceable::SharedTraceable const& right) {
         AABB boxLeft = left->boundingBox();
         AABB boxRight = right->boundingBox();
         if(boxLeft.min()[axis_] - boxRight.max()[axis_] < 1. ) {
            return -1;
         } else {
            return 1;
         }
      }

      private:
      Size axis_;
   };

   public:
   TraceableNode(SharedTraceableVector& references)
       : refLeft_()
       , refRight_()
       , box_() {
      const Int randomAxis = Int(3. * drand48());
      Size size = references.size();
      Comparator comparator(randomAxis);
      //std::sort(references.begin(), references.end(), comparator);
      if(size == 1) {
         refLeft_ = references[0];
         refRight_ = nullptr;
         box_ = refLeft_->boundingBox();
      } else if(size == 2) {
         refLeft_ = references[0];
         refRight_ = references[1];
         box_ = refLeft_->boundingBox() + refRight_->boundingBox();
      } else if(size > 2) {
         Size middle = size / 2;
         SharedTraceableVector leftRefs(references.begin(), references.begin() + middle);
         refLeft_ = std::make_shared<TraceableNode>(leftRefs);
         SharedTraceableVector rightRefs(references.begin() + middle, references.end());
         refRight_ = std::make_shared<TraceableNode>(rightRefs);
         box_ = refLeft_->boundingBox() + refRight_->boundingBox();
      } else {
          throw( std::logic_error( "TraceableNode: invalid size of input data!" ));
      }
   }

   AABB boundingBox() const { return box_; }

   bool hit(Ray const& ray, Real const& lowerLimit, Real const& upperLimit,
            TraceData& traceDataOut) const {
      if(  ! box_.hit(ray, lowerLimit, upperLimit) ) {
         return false;
      }
      TraceData dataLeft, dataRight;

      bool hitLeft = refLeft_->hit(ray, lowerLimit, upperLimit, dataLeft);
      if(refRight_ == nullptr) {
         if(hitLeft) {
            traceDataOut = dataLeft;
            return true;
         }
         return false;
      }

      bool hitRight = refRight_->hit(ray, lowerLimit, upperLimit, dataRight);
      if(hitLeft && hitRight) {
         if(dataLeft.positionOnRay < dataRight.positionOnRay) {
            traceDataOut = dataLeft;
         } else {
            traceDataOut = dataRight;
         }
      } else if(hitLeft) {
         traceDataOut = dataLeft;
      } else if(hitRight) {
         traceDataOut = dataRight;
      } else {
          return false;
      }
      return true;
   }

   private:
   SharedTraceable refLeft_;
   SharedTraceable refRight_;
   AABB box_;
};
}

#endif
