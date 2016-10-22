/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZTRACE_CAMERA_H
#define ZTRACE_CAMERA_H

#include <cmath> // M_PI

#include "Random.h"
#include "Ray.h"
#include "Vector.h"

namespace ztrace {
class Camera {
   public:
   Camera()
       : position_()
       , pointingDirection_(0., 0., -1.)
       , pointingNormalX_(1., 0., 0.)
       , pointingNormalY_(0., 1., 0.)
       , width_(1.6)
       , height_(0.9)
       , focalLength_(1.0)
       , aperture_(0.01) {}

   Camera(Vector const& position, Vector const& pointingDirection,
          Real const& width = 1.6, Real const& height = 0.9,
          Real const& focalLength = 1.0, Real const& aperture = 0.05)
       : position_(position)
       , pointingDirection_(pointingDirection)
       , pointingNormalX_()
       , pointingNormalY_()
       , width_(width)
       , height_(height)
       , focalLength_(focalLength)
       , aperture_(aperture) {
      updateState();
   }

   void updateState() {
      // earths normal (to level the camera)
      static Vector earthNormal(0., -1., 0.);
      pointingDirection_.makeUnitVector();
      pointingNormalX_ = -cross(pointingDirection_, earthNormal);
      pointingNormalY_ = -cross(pointingDirection_, pointingNormalX_);
      pointingNormalX_.makeUnitVector();
      pointingNormalY_.makeUnitVector();
   }

   void emitRaySingle(Real const& screenX, Real const& screenY,
                      Ray& emittedRayOut) const {
      Vector offset = offsetAparture(randomCylinderCoordinate());
      Vector target = pointingDirection_ * focalLength_;
      target += pointingNormalX_ * width_ * (screenX - 0.5);
      target += pointingNormalY_ * height_ * (0.5 - screenY);
      emittedRayOut = Ray{position_ + offset, target - offset};
   }
   void emitRayPair(Real const& screenX, Real const& screenY, Ray& emittedRayOut1,
                    Ray& emittedRayOut2) const {
      Vector cylinderCoordinates = randomCylinderCoordinate();
      Vector target = moveToCoordinate( pointingDirection_ * focalLength_, screenX, screenY );
      Vector offset;

      offset = offsetAparture(cylinderCoordinates);
      emittedRayOut1 = Ray{position_ + offset, target - offset};
      offset =
          offsetAparture(cylinderCoordinates + Vector{0., M_PI, 0.}); // radius x, angle y
      emittedRayOut2 = Ray{position_ + offset, target - offset};
   }

   private:
   // radius x, angle y
   Vector const & moveToCoordinate( Vector && target, Real const & screenX, Real const & screenY ) const {
      target += pointingNormalX_ * width_ * (screenX - 0.5);
      target += pointingNormalY_ * height_ * (0.5 - screenY);
      return target;
   }
   Vector const offsetAparture(Vector const& cylinderCoordinatesRAH) const {
      return cylinderCoordinatesRAH.x() * aperture_
             * (cos(cylinderCoordinatesRAH.y()) * pointingNormalX_
                + sin(cylinderCoordinatesRAH.y()) * pointingNormalY_);
   }
   Vector const randomCylinderCoordinate() const {
      return Vector{random(), random() * M_PI, 0.0};
   }

   Vector position_;
   Vector pointingDirection_;
   Vector pointingNormalX_;
   Vector pointingNormalY_;
   Real width_; // how much fisheye it is
   Real height_;
   Real focalLength_;
   Real aperture_;
};
}

#endif // ZTRACE_CAMERA_H
