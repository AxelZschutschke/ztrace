/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZTRACE_VECTOR_H
#define ZTRACE_VECTOR_H

#include <array>   // std::array<>
#include <ostream> // operator<<
#include "Types.h" // Real, Int, Size

namespace ztrace
{

/*! \brief 3D Vector class
 *
 * ztrace::Vector is an extension of std::array<Real,3> plus math operators!
 */
class Vector
{
    using VectorType = std::array<Real, 3>;

public:
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// contructors/destructors
    //// most are left unimplemented -> compiler does a better job than me
    //// also automatically makes vector an atomic
    //
    Vector();
    Vector(Real const& x, Real const& y, Real const& z);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// external vector algebra helper functions
    //
    Vector abs() const;      //!< absolute values for each vector element
    Real len() const;        //!< length of vector
    Real sum() const;        //!< sum over all vector elements
    Real lenSquared() const; //!< squared length of vector

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// vector modification functions
    //
    Vector const& makeUnitVector(); //!< change this vector to length = 1 while keeping direction
    Vector const& limitMax(Real const& limit = 1.); //!< limit maximum value of vector
    Vector const& limitMin(Real const& limit = 0.); //!< limit minimum value of vector

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// element-wise access operators
    //
    Real const& x() const { return vec_[0]; }
    Real const& y() const { return vec_[1]; }
    Real const& z() const { return vec_[2]; }
    Real& x() { return vec_[0]; }
    Real& y() { return vec_[1]; }
    Real& z() { return vec_[2]; }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// global access operators
    //
    VectorType::const_iterator begin() const;
    VectorType::iterator begin();
    VectorType::const_iterator end() const;
    VectorType::iterator end();

    Real const& operator[](Size const& idx) const;
    Real& operator[](Size const& idx);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// unary operators
    //
    Vector const operator+() const;
    Vector const operator-() const;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// operation/assignment operators
    //
    Vector const& operator+=(Vector const& rhs);
    Vector const& operator*=(Vector const& rhs);
    Vector const& operator-=(Vector const& rhs);
    Vector const& operator/=(Vector const& rhs);
    Vector const& operator+=(Real const& rhs);
    Vector const& operator*=(Real const& rhs);
    Vector const& operator-=(Real const& rhs);
    Vector const& operator/=(Real const& rhs);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// internal/external calculus operators
    //
    Vector operator+(Vector const& rhs) const;
    Vector operator*(Vector const& rhs) const;
    Vector operator-(Vector const& rhs) const;
    Vector operator/(Vector const& rhs) const;
    Vector operator+(Vector&& rhs) const;
    Vector operator*(Vector&& rhs) const;
    Vector operator-(Vector&& rhs) const;
    Vector operator/(Vector&& rhs) const;
    Vector operator+(Real const& rhs) const;
    Vector operator*(Real const& rhs) const;
    Vector operator-(Real const& rhs) const;
    Vector operator/(Real const& rhs) const;

private:
    VectorType vec_;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// stream put operator<< - not a friend!
//
std::ostream& operator<<(std::ostream& out, Vector const& vector);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// vector algebra external
//
Real dot(Vector const& a, Vector const& b); //!< dot product, reference version
Real dot(Vector const& a, Vector&& b);      //!< dot product, destructive version
Real dot(Vector&& a, Vector const& b);      //!< dot product, destructive version
Vector const cross(Vector const& a, Vector const& b); //!< cross product

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// vector external calculus operators
//
Vector const operator+(Real const& lhs, Vector const& rhs);
Vector const operator*(Real const& lhs, Vector const& rhs);
Vector const operator-(Real const& lhs, Vector const& rhs);
Vector const operator/(Real const& lhs, Vector const& rhs);
Vector const& operator+(Real const& lhs, Vector&& rhs);
Vector const& operator*(Real const& lhs, Vector&& rhs);
Vector const& operator-(Real const& lhs, Vector&& rhs);
Vector const& operator/(Real const& lhs, Vector&& rhs);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// default vectors supplied by library
//
static Vector const unit{1., 1., 1.};
static Vector const zero{0., 0., 0.};

} // namespace ztrace

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// including the actual implementation
#include "Vector_impl.h"

#endif // ZTRACE_VECTOR_H
