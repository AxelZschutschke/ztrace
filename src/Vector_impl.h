/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZTRACE_VECTOR_IMPL_H
#define ZTRACE_VECTOR_IMPL_H

#ifndef ZTRACE_VECTOR_H
// not really nice and/or necessary, but helps YouCompleteMe and clang code analysis
#include "Vector.h"
#endif // ZTRACE_VECTOR_H

#include "Types.h"
#include <array>
#include <cmath>
#include <ostream>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// contructors/destructors
//// most are left unimplemented -> compiler does a better job than me
//// also automatically makes vector an atomic
//
ztrace::Vector::Vector()
    : vec_{{0., 0., 0.}}
{
}

ztrace::Vector::Vector(ztrace::Real const& x, ztrace::Real const& y, ztrace::Real const& z)
    : vec_{{x, y, z}}
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// external vector algebra helper functions
//
ztrace::Vector ztrace::Vector::abs() const
{
    ztrace::Vector result;
    std::transform(vec_.begin(), vec_.end(), result.vec_.begin(),
                   [](ztrace::Real const& lhs) { return lhs > 0. ? lhs : -lhs; });
    return result;
}
ztrace::Real ztrace::Vector::len() const { return sqrt(lenSquared()); }
ztrace::Real ztrace::Vector::sum() const { return vec_[0] + vec_[1] + vec_[2]; }
ztrace::Real ztrace::Vector::lenSquared() const
{
    return vec_[0] * vec_[0] + vec_[1] * vec_[1] + vec_[2] * vec_[2];
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// vector modication functions
//
ztrace::Vector const& ztrace::Vector::makeUnitVector()
{
    operator/=(len());
    return *this;
}
ztrace::Vector const& ztrace::Vector::limitMax(ztrace::Real const& limit)
{
    std::transform(vec_.begin(), vec_.end(), vec_.begin(),
                   [limit](ztrace::Real const& lhs) { return lhs > limit ? limit : lhs; });
    return *this;
}
ztrace::Vector const& ztrace::Vector::limitMin(ztrace::Real const& limit)
{
    std::transform(vec_.begin(), vec_.end(), vec_.begin(),
                   [limit](ztrace::Real const& lhs) { return lhs < limit ? limit : lhs; });
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// global access operators
//
ztrace::Real const& ztrace::Vector::operator[](Size const& idx) const
{
    return vec_[idx];
}
ztrace::Real& ztrace::Vector::operator[](Size const& idx) { return vec_[idx]; }

ztrace::Vector::VectorType::const_iterator ztrace::Vector::begin() const
{
    return vec_.begin();
}
ztrace::Vector::VectorType::iterator ztrace::Vector::begin() { return vec_.begin(); }
ztrace::Vector::VectorType::const_iterator ztrace::Vector::end() const
{
    return vec_.end();
}
ztrace::Vector::VectorType::iterator ztrace::Vector::end() { return vec_.end(); }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// vector calculus operators -- operator/assignment
//
ztrace::Vector const ztrace::Vector::operator+() const { return *this; }
ztrace::Vector const ztrace::Vector::operator-() const
{
    ztrace::Vector result;
    std::transform(vec_.begin(), vec_.end(), result.vec_.begin(),
                   [](ztrace::Real const& v) { return -v; });
    return result;
}
ztrace::Vector const& ztrace::Vector::operator+=(ztrace::Vector const& rhs)
{
    std::transform(vec_.begin(), vec_.end(), rhs.vec_.begin(), vec_.begin(),
                   std::plus<ztrace::Real>());
    return *this;
}
ztrace::Vector const& ztrace::Vector::operator*=(ztrace::Vector const& rhs)
{
    std::transform(vec_.begin(), vec_.end(), rhs.vec_.begin(), vec_.begin(),
                   std::multiplies<ztrace::Real>());
    return *this;
}
ztrace::Vector const& ztrace::Vector::operator-=(ztrace::Vector const& rhs)
{
    std::transform(vec_.begin(), vec_.end(), rhs.vec_.begin(), vec_.begin(),
                   std::minus<ztrace::Real>());
    return *this;
}
ztrace::Vector const& ztrace::Vector::operator/=(ztrace::Vector const& rhs)
{
    std::transform(vec_.begin(), vec_.end(), rhs.vec_.begin(), vec_.begin(),
                   std::divides<ztrace::Real>());
    return *this;
}
ztrace::Vector const& ztrace::Vector::operator+=(ztrace::Real const& rhs)
{
    std::transform(vec_.begin(), vec_.end(), vec_.begin(),
                   [rhs](ztrace::Real const& lhs) { return lhs + rhs; });
    return *this;
}
ztrace::Vector const& ztrace::Vector::operator*=(ztrace::Real const& rhs)
{
    std::transform(vec_.begin(), vec_.end(), vec_.begin(),
                   [rhs](ztrace::Real const& lhs) { return lhs * rhs; });
    return *this;
}
ztrace::Vector const& ztrace::Vector::operator-=(ztrace::Real const& rhs)
{
    std::transform(vec_.begin(), vec_.end(), vec_.begin(),
                   [rhs](ztrace::Real const& lhs) { return lhs - rhs; });
    return *this;
}
ztrace::Vector const& ztrace::Vector::operator/=(ztrace::Real const& rhs)
{
    std::transform(vec_.begin(), vec_.end(), vec_.begin(),
                   [rhs](ztrace::Real const& lhs) { return lhs / rhs; });
    return *this;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// vector calculus operators -- internal/external operators
//
ztrace::Vector ztrace::Vector::operator+(ztrace::Vector const& rhs) const
{
    ztrace::Vector result(*this);
    result += rhs;
    return result;
}
ztrace::Vector ztrace::Vector::operator*(ztrace::Vector const& rhs) const
{
    ztrace::Vector result(*this);
    result *= rhs;
    return result;
}
ztrace::Vector ztrace::Vector::operator-(ztrace::Vector const& rhs) const
{
    ztrace::Vector result(*this);
    result -= rhs;
    return result;
}
ztrace::Vector ztrace::Vector::operator/(ztrace::Vector const& rhs) const
{
    ztrace::Vector result(*this);
    result /= rhs;
    return result;
}
ztrace::Vector ztrace::Vector::operator+(ztrace::Vector&& rhs) const
{
    rhs += *this;
    return rhs;
}
ztrace::Vector ztrace::Vector::operator*(ztrace::Vector&& rhs) const
{
    rhs *= *this;
    return rhs;
}
ztrace::Vector ztrace::Vector::operator-(ztrace::Vector&& rhs) const
{
    std::transform(
        vec_.begin(), vec_.end(), rhs.vec_.begin(), rhs.vec_.begin(),
        [](ztrace::Real const& lhs, ztrace::Real const& rhs) { return lhs - rhs; });
    return rhs;
}
ztrace::Vector ztrace::Vector::operator/(ztrace::Vector&& rhs) const
{
    std::transform(
        vec_.begin(), vec_.end(), rhs.vec_.begin(), rhs.vec_.begin(),
        [](ztrace::Real const& lhs, ztrace::Real const& rhs) { return lhs / rhs; });
    return rhs;
}
ztrace::Vector ztrace::Vector::operator+(ztrace::Real const& rhs) const
{
    ztrace::Vector result(*this);
    result += rhs;
    return result;
}
ztrace::Vector ztrace::Vector::operator*(ztrace::Real const& rhs) const
{
    ztrace::Vector result(*this);
    result *= rhs;
    return result;
}
ztrace::Vector ztrace::Vector::operator-(ztrace::Real const& rhs) const
{
    ztrace::Vector result(*this);
    result -= rhs;
    return result;
}
ztrace::Vector ztrace::Vector::operator/(ztrace::Real const& rhs) const
{
    ztrace::Vector result(*this);
    result /= rhs;
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// stream put operator<< - not a friend!
//
std::ostream& ztrace::operator<<(std::ostream& out, ztrace::Vector const& vector)
{
    out << vector.x() << " " << vector.y() << " " << vector.z();
    return out;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// vector algebra external
//
ztrace::Real ztrace::dot(ztrace::Vector const& a, ztrace::Vector const& b)
{
    return (a * b).sum();
}
ztrace::Real ztrace::dot(ztrace::Vector const& a, ztrace::Vector&& b) { return (b *= a).sum(); }
ztrace::Real ztrace::dot(ztrace::Vector&& a, ztrace::Vector const& b) { return (a *= b).sum(); }
ztrace::Vector const ztrace::cross(ztrace::Vector const& a, ztrace::Vector const& b)
{
    return ztrace::Vector{a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(),
                          a.x() * b.y() - a.y() * b.z()};
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// vector external calculus operators
//
ztrace::Vector const ztrace::operator+(ztrace::Real const& lhs, ztrace::Vector const& rhs)
{
    return rhs + lhs;
}
ztrace::Vector const ztrace::operator*(ztrace::Real const& lhs, ztrace::Vector const& rhs)
{
    return rhs * lhs;
}
ztrace::Vector const ztrace::operator-(ztrace::Real const& lhs, ztrace::Vector const& rhs)
{
    ztrace::Vector result{lhs, lhs, lhs};
    return result -= rhs;
}
ztrace::Vector const ztrace::operator/(ztrace::Real const& lhs, ztrace::Vector const& rhs)
{
    ztrace::Vector result{lhs, lhs, lhs};
    return result /= rhs;
}
ztrace::Vector const& ztrace::operator+(ztrace::Real const& lhs, ztrace::Vector&& rhs)
{
    return rhs += lhs;
}
ztrace::Vector const& ztrace::operator*(ztrace::Real const& lhs, ztrace::Vector&& rhs)
{
    return rhs *= lhs;
}
ztrace::Vector const& ztrace::operator-(ztrace::Real const& lhs, ztrace::Vector&& rhs)
{
    rhs.x() = lhs - rhs.x();
    rhs.y() = lhs - rhs.y();
    rhs.z() = lhs - rhs.z();
    return rhs;
}
ztrace::Vector const& ztrace::operator/(ztrace::Real const& lhs, ztrace::Vector&& rhs)
{
    rhs.x() = lhs / rhs.x();
    rhs.y() = lhs / rhs.y();
    rhs.z() = lhs / rhs.z();
    return rhs;
}

#endif // ZTRACE_VECTOR_IMPL_H
