#ifndef ZTRACE_TRACEABLE_NODE_H
#define ZTRACE_TRACEABLE_NODE_H

#include "AABB.h"
#include "Random.h"
#include "Traceable.h"

namespace ztrace {

class TraceableNode : public Traceable {
   class Comparator {
      public:
      Comparator(Size const axis)
          : axis_(axis) {}

      bool operator()(Traceable::SharedTraceable const& left,
                     Traceable::SharedTraceable const& right) {
         AABB boxLeft = left->boundingBox();
         AABB boxRight = right->boundingBox();
         return boxLeft.min()[axis_] > boxRight.max()[axis_];
      }
      private:
      Size const axis_;
   };

   public:
   TraceableNode(SharedTraceableVector& references)
       : refLeft_()
       , refRight_()
       , box_() {
      const Size randomAxis = Size(3. * random());
      Size nObjects = references.size();
      Comparator comparator(randomAxis);
      std::sort(references.begin(), references.end(), comparator);
      if(nObjects == 1) {
         refLeft_ = references[0];
         refRight_ = nullptr;
         box_ = refLeft_->boundingBox();
      } else if(nObjects == 2) {
         refLeft_ = references[0];
         refRight_ = references[1];
         box_ = refLeft_->boundingBox() + refRight_->boundingBox();
      } else if(nObjects > 2) {
         Size middle = nObjects / 2;
         SharedTraceableVector leftRefs(references.begin(), references.begin() + middle);
         refLeft_ = std::make_shared<TraceableNode>(leftRefs);
         SharedTraceableVector rightRefs(references.begin() + middle, references.end());
         refRight_ = std::make_shared<TraceableNode>(rightRefs);
         box_ = refLeft_->boundingBox() + refRight_->boundingBox();
      } else {
         throw(std::logic_error("TraceableNode: invalid size of input data!"));
      }
      std::cout << "size: " << nObjects << " " << box_.min() << " / " << box_.max() << std::endl;
   }

   AABB boundingBox() const { return box_; }

   bool hit(Ray const& ray, Real const& lowerLimit, Real const& upperLimit,
            TraceData& traceDataOut) const {
      if(!box_.hit(ray, lowerLimit, upperLimit)) {
         return false;
      }
      TraceData dataLeft;

      bool hitLeft = refLeft_->hit(ray, lowerLimit, upperLimit, dataLeft);
      if(refRight_ == nullptr) {
         if(hitLeft) {
            traceDataOut = dataLeft;
            return true;
         }
         return false;
      }

      TraceData dataRight;
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
