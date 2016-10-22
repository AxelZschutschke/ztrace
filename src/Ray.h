/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZTRACER_RAY_H
#define ZTRACER_RAY_H

#include "Vector.h"

namespace ztrace {

class Ray {
   public:
   Ray()
       : A_()
       , B_() {}
   Ray(Vector const& A, Vector const& B)
       : A_(A)
       , B_(B) {}
   Ray(Vector const& A, Vector const&& B)
       : A_(A)
       , B_(std::move(B)) {}
   Ray(Vector const&& A, Vector const&& B)
       : A_(std::move(A))
       , B_(std::move(B)) {}
   Ray(Ray const& rhs)
       : A_(rhs.A_)
       , B_(rhs.B_) {}
   Ray(Ray&& rhs)
       : A_(std::move(rhs.A_))
       , B_(std::move(rhs.B_)) {}
   Ray const& operator=(Ray const& rhs) {
      A_ = rhs.A_;
      B_ = rhs.B_;
      return *this;
   }
   Ray const& operator=(Ray const&& rhs) {
      A_ = std::move(rhs.A_);
      B_ = std::move(rhs.B_);
      return *this;
   }

   Vector& origin() { return A_; }
   Vector const& origin() const { return A_; }
   Vector& direction() { return B_; }
   Vector const& direction() const { return B_; }
   Vector const positionLength(Real const& length) const { return A_ + B_ * length; }

   private:
   Vector A_;
   Vector B_;
};

class ShadowRay : public Ray {
   public:
   ShadowRay()
       : Ray()
       , specular_() {}
   ShadowRay(Vector const& A, Vector const& B, bool specular = false)
       : Ray(A, B)
       , specular_(specular) {}
   ShadowRay(Vector const&& A, Vector const&& B, bool specular = false)
       : Ray(A, B)
       , specular_(specular) {}
   ShadowRay(ShadowRay const& rhs, bool specular = false)
       : Ray(rhs)
       , specular_(specular) {}
   ShadowRay(ShadowRay const&& rhs, bool specular = false)
       : Ray(rhs)
       , specular_(specular) {}
   Ray const& operator=(ShadowRay const& rhs) {
      Ray::operator=(rhs);
      specular_ = rhs.specular_;
      return *this;
   }
   Ray const& operator=(ShadowRay const&& rhs) {
      Ray::operator=(rhs);
      specular_ = rhs.specular_;
      return *this;
   }

   bool& specular() { return specular_; }
   bool const& specular() const { return specular_; }

   private:
   bool specular_;
};
}

#endif // ZTRACER_RAY_H
