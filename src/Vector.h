//
// Created by Axel Zschutschke on 10/8/16.
//

#ifndef ZTRACER_VECTOR_H
#define ZTRACER_VECTOR_H

#include <ostream>
#include <cmath>
#include "Types.h"

namespace ztrace {
    class Vector {
        using ostream = std::ostream;
    public:
        Vector()
                : x_(0.)
                , y_(0.)
                , z_(0.)
        {}

        Vector( Real const & x, Real const & y, Real const & z )
                : x_(x)
                , y_(y)
                , z_(z)
        {}

        friend ostream & operator<<( ostream & out, Vector const & vector ){
            out << vector.x_ << " " << vector.y_ << " " << vector.z_;
            return out;
        }

        Real const & x() const { return x_; }
        Real const & y() const { return y_; }
        Real const & z() const { return z_; }
        Vector const operator+( ) const { return *this; }
        Vector operator-( ) const { Vector result( *this ); result.x_ = -result.x_; result.y_ = -result.y_; result.z_ = -result.z_; return result; }
        Vector const & operator+=( Vector const & rhs ){ x_ += rhs.x_; y_ += rhs.y_; z_ += rhs.z_; return *this; }
        Vector const & operator*=( Vector const & rhs ){ x_ *= rhs.x_; y_ *= rhs.y_; z_ *= rhs.z_; return *this; }
        Vector const & operator-=( Vector const & rhs ){ x_ -= rhs.x_; y_ -= rhs.y_; z_ -= rhs.z_; return *this; }
        Vector const & operator/=( Vector const & rhs ){ x_ /= rhs.x_; y_ /= rhs.y_; z_ /= rhs.z_; return *this; }
        Vector const & operator+=( Real const & rhs ){ x_ += rhs; y_ += rhs; z_ += rhs; return *this; }
        Vector const & operator*=( Real const & rhs ){ x_ *= rhs; y_ *= rhs; z_ *= rhs; return *this; }
        Vector const & operator-=( Real const & rhs ){ x_ -= rhs; y_ -= rhs; z_ -= rhs; return *this; }
        Vector const & operator/=( Real const & rhs ){ x_ /= rhs; y_ /= rhs; z_ /= rhs; return *this; }
        Vector operator+( Vector const & rhs ) const { Vector result( *this ); result += rhs; return result; }
        Vector operator*( Vector const & rhs ) const { Vector result( *this ); result *= rhs; return result; }
        Vector operator-( Vector const & rhs ) const { Vector result( *this ); result -= rhs; return result; }
        Vector operator/( Vector const & rhs ) const { Vector result( *this ); result /= rhs; return result; }
        Vector operator+( Real const & rhs ) const { Vector result( *this ); result += rhs; return result; }
        Vector operator*( Real const & rhs ) const { Vector result( *this ); result *= rhs; return result; }
        Vector operator-( Real const & rhs ) const { Vector result( *this ); result -= rhs; return result; }
        Vector operator/( Real const & rhs ) const { Vector result( *this ); result /= rhs; return result; }

        Vector abs() const { Vector result(  x_ < 0. ? -x_ : x_, y_ < 0. ? -y_ : y_, z_ < 0. ? -z_ : z_ ); return result; }
        Real len() const { return sqrt( lenSquared() ); }
        Real sum() const { return x_ + y_ + z_; }
        Real lenSquared() const { return x_ * x_ + y_ * y_ + z_ * z_; }
        Vector const & makeUnitVector( ) { operator/=( len() ); return *this; }

    private:
        Real x_;
        Real y_;
        Real z_;
    };

    Real const dot( Vector const & a, Vector const & b ){ return ( a * b ).sum(); }

    Vector const operator+( Real const & lhs, Vector const & rhs ){ return rhs + lhs; }
    Vector const operator*( Real const & lhs, Vector const & rhs ){ return rhs * lhs; }
    Vector const operator-( Real const & lhs, Vector const & rhs ){ return rhs - lhs; }
    Vector const operator/( Real const & lhs, Vector const & rhs ){ return rhs / lhs; }
}


#endif //ZTRACER_VECTOR_H
