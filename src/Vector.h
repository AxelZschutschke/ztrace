//
// Created by Axel Zschutschke on 10/8/16.
//

#ifndef ZTRACER_VECTOR_H
#define ZTRACER_VECTOR_H

#include "Types.h"
#include <array>
#include <cmath>
#include <ostream>

namespace ztrace {

class Vector {
   using ostream = std::ostream;

   public:
   Vector()
       : vec_{{0., 0., 0.}} {}

   Vector(Real const& x, Real const& y, Real const& z)
       : vec_{{x, y, z}} {}
   Vector(Vector const& rhs)
       : vec_{rhs.vec_} {}
   Vector(Vector&& rhs)
       : vec_{std::move(rhs.vec_)} {}
   Vector const& operator=(Vector const& rhs) {
      vec_ = rhs.vec_;
      return *this;
   }
   Vector const& operator=(Vector&& rhs) {
      vec_ = std::move(rhs.vec_);
      return *this;
   }

   friend ostream& operator<<(ostream& out, Vector const& vector) {
      out << vector.x() << " " << vector.y() << " " << vector.z();
      return out;
   }

   Real const& x() const { return vec_[0]; }
   Real const& y() const { return vec_[1]; }
   Real const& z() const { return vec_[2]; }
   Real& x() { return vec_[0]; }
   Real& y() { return vec_[1]; }
   Real& z() { return vec_[2]; }

   Vector const operator+() const { return *this; }
   Vector const operator-() const {
      Vector result;
      std::transform(vec_.begin(), vec_.end(), result.vec_.begin(),
                     [](Real const& v) { return -v; });
      return result;
   }
   Vector const& operator+=(Vector const& rhs) {
      std::transform(vec_.begin(), vec_.end(), rhs.vec_.begin(), vec_.begin(),
                     std::plus<Real>());
      return *this;
   }
   Vector const& operator*=(Vector const& rhs) {
      std::transform(vec_.begin(), vec_.end(), rhs.vec_.begin(), vec_.begin(),
                     std::multiplies<Real>());
      return *this;
   }
   Vector const& operator-=(Vector const& rhs) {
      std::transform(vec_.begin(), vec_.end(), rhs.vec_.begin(), vec_.begin(),
                     std::minus<Real>());
      return *this;
   }
   Vector const& operator/=(Vector const& rhs) {
      std::transform(vec_.begin(), vec_.end(), rhs.vec_.begin(), vec_.begin(),
                     std::divides<Real>());
      return *this;
   }
   Vector const& operator+=(Real const& rhs) {
      std::transform(vec_.begin(), vec_.end(), vec_.begin(),
                     [rhs](Real const& lhs) { return lhs + rhs; });
      return *this;
   }
   Vector const& operator*=(Real const& rhs) {
      std::transform(vec_.begin(), vec_.end(), vec_.begin(),
                     [rhs](Real const& lhs) { return lhs * rhs; });
      return *this;
   }
   Vector const& operator-=(Real const& rhs) {
      std::transform(vec_.begin(), vec_.end(), vec_.begin(),
                     [rhs](Real const& lhs) { return lhs - rhs; });
      return *this;
   }
   Vector const& operator/=(Real const& rhs) {
      std::transform(vec_.begin(), vec_.end(), vec_.begin(),
                     [rhs](Real const& lhs) { return lhs / rhs; });
      return *this;
   }
   Vector operator+(Vector const& rhs) const {
      Vector result(*this);
      result += rhs;
      return result;
   }
   Vector operator*(Vector const& rhs) const {
      Vector result(*this);
      result *= rhs;
      return result;
   }
   Vector operator-(Vector const& rhs) const {
      Vector result(*this);
      result -= rhs;
      return result;
   }
   Vector operator/(Vector const& rhs) const {
      Vector result(*this);
      result /= rhs;
      return result;
   }
   Vector operator+(Vector&& rhs) const {
      rhs += *this;
      return rhs;
   }
   Vector operator*(Vector&& rhs) const {
      rhs *= *this;
      return rhs;
   }
   Vector operator-(Vector&& rhs) const {
      std::transform(vec_.begin(), vec_.end(), rhs.vec_.begin(), rhs.vec_.begin(),
                     [](Real const& lhs, Real const& rhs) { return lhs - rhs; });
      return rhs;
   }
   Vector operator/(Vector&& rhs) const {
      std::transform(vec_.begin(), vec_.end(), rhs.vec_.begin(), rhs.vec_.begin(),
                     [](Real const& lhs, Real const& rhs) { return lhs / rhs; });
      return rhs;
   }
   Vector operator+(Real const& rhs) const {
      Vector result(*this);
      result += rhs;
      return result;
   }
   Vector operator*(Real const& rhs) const {
      Vector result(*this);
      result *= rhs;
      return result;
   }
   Vector operator-(Real const& rhs) const {
      Vector result(*this);
      result -= rhs;
      return result;
   }
   Vector operator/(Real const& rhs) const {
      Vector result(*this);
      result /= rhs;
      return result;
   }
   Real const & operator[](Size const& idx) const {
      return vec_[idx];
   }
   Real & operator[](Size const& idx) {
      return vec_[idx];
   }

   Vector abs() const {
      Vector result;
      std::transform(vec_.begin(), vec_.end(), result.vec_.begin(),
                     [](Real const& lhs) { return lhs > 0. ? lhs : -lhs; });
      return result;
   }
   Real len() const { return sqrt(lenSquared()); }
   Real sum() const { return vec_[0] + vec_[1] + vec_[2]; }
   Real lenSquared() const {
      return vec_[0] * vec_[0] + vec_[1] * vec_[1] + vec_[2] * vec_[2];
   }
   Vector const& makeUnitVector() {
      operator/=(len());
      return *this;
   }
   void limitMax(Real const& limit = 1.) {
      std::transform(vec_.begin(), vec_.end(), vec_.begin(),
                     [limit](Real const& lhs) { return lhs > limit ? limit : lhs; });
   }
   void limitMin(Real const& limit = 0.) {
      std::transform(vec_.begin(), vec_.end(), vec_.begin(),
                     [limit](Real const& lhs) { return lhs < limit ? limit : lhs; });
   }

   private:
   std::array<Real, 3> vec_;
};

Real dot(Vector const& a, Vector const& b) {
   return (a * b).sum();
}
Real dot(Vector const& a, Vector&& b) {
   return (b *= a).sum();
}
Real dot(Vector&& a, Vector const& b) {
   return (a *= b).sum();
}
Vector const cross(Vector const& a, Vector const& b) {
   return Vector{a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(),
                 a.x() * b.y() - a.y() * b.z()};
}

Vector const operator+(Real const& lhs, Vector const& rhs) {
   return rhs + lhs;
}
Vector const operator*(Real const& lhs, Vector const& rhs) {
   return rhs * lhs;
}
Vector const operator-(Real const& lhs, Vector const& rhs) {
   return rhs - lhs;
}
Vector const operator/(Real const& lhs, Vector const& rhs) {
   return rhs / lhs;
}
Vector const& operator+(Real const& lhs, Vector&& rhs) {
   return rhs += lhs;
}
Vector const& operator*(Real const& lhs, Vector&& rhs) {
   return rhs *= lhs;
}
Vector const& operator-(Real const& lhs, Vector&& rhs) {
   rhs.x() = lhs - rhs.x();
   rhs.y() = lhs - rhs.y();
   rhs.z() = lhs - rhs.z();
   return rhs;
}
Vector const& operator/(Real const& lhs, Vector&& rhs) {
   rhs.x() = lhs / rhs.x();
   rhs.y() = lhs / rhs.y();
   rhs.z() = lhs / rhs.z();
   return rhs;
}

static Vector const unit{1., 1., 1.};
static Vector const zero{0., 0., 0.};
}


#endif // ZTRACER_VECTOR_H
