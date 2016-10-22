/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZTRACE_RAY_H
#define ZTRACE_RAY_H

#include "Vector.h"

namespace ztrace
{

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Declaration
//

/*! \brief Light Ray containing origin and direction of single light beam */
class Ray
{
public:
    Ray();
    Ray(Vector const& A, Vector const& B);
    Ray(Vector const& A, Vector const&& B);
    Ray(Vector const&& A, Vector const&& B);
    Ray(Ray const& rhs);
    Ray(Ray&& rhs);
    Ray const& operator=(Ray const& rhs);
    Ray const& operator=(Ray const&& rhs);

    Vector& origin() { return A_; }
    Vector const& origin() const { return A_; }
    Vector& direction() { return B_; }
    Vector const& direction() const { return B_; }
    Vector const positionAtLength(Real const& length) const { return A_ + B_ * length; }

private:
    Vector A_;
    Vector B_;
};

} // namespace ztrace

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Implementation
//
ztrace::Ray::Ray()
    : A_()
    , B_()
{
}
ztrace::Ray::Ray(Vector const& A, Vector const& B)
    : A_(A)
    , B_(B)
{
}
ztrace::Ray::Ray(Vector const& A, Vector const&& B)
    : A_(A)
    , B_(std::move(B))
{
}
ztrace::Ray::Ray(Vector const&& A, Vector const&& B)
    : A_(std::move(A))
    , B_(std::move(B))
{
}
ztrace::Ray::Ray(Ray const& rhs)
    : A_(rhs.A_)
    , B_(rhs.B_)
{
}
ztrace::Ray::Ray(Ray&& rhs)
    : A_(std::move(rhs.A_))
    , B_(std::move(rhs.B_))
{
}
ztrace::Ray const& ztrace::Ray::operator=(Ray const& rhs)
{
    A_ = rhs.A_;
    B_ = rhs.B_;
    return *this;
}
ztrace::Ray const& ztrace::Ray::operator=(Ray const&& rhs)
{
    A_ = std::move(rhs.A_);
    B_ = std::move(rhs.B_);
    return *this;
}

#endif // ZTRACE_RAY_H
