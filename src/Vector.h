//
// Created by Axel Zschutschke on 10/8/16.
//

#ifndef ZTRACER_VECTOR_H
#define ZTRACER_VECTOR_H

#include <ostream>
#include <cmath>
#include <vector>
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
        Vector( Vector const & rhs )
                : x_(rhs.x_)
                , y_(rhs.y_)
                , z_(rhs.z_)
        {}
        Vector const & operator=( Vector const & rhs ){
            x_ = rhs.x_;
            y_ = rhs.y_;
            z_ = rhs.z_;
            return *this;
        }

        friend ostream & operator<<( ostream & out, Vector const & vector ){
            out << vector.x_ << " " << vector.y_ << " " << vector.z_;
            return out;
        }

        Real const & x() const { return x_; }
        Real const & y() const { return y_; }
        Real const & z() const { return z_; }
        Real & x() { return x_; }
        Real & y() { return y_; }
        Real & z() { return z_; }

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
        Vector operator+( Vector && rhs ) const { rhs += *this; return rhs; }
        Vector operator*( Vector && rhs ) const { rhs *= *this; return rhs; }
        Vector operator-( Vector && rhs ) const { rhs.x_ = x_ - rhs.x_; rhs.y_ = y_ - rhs.y_; rhs.z_ = z_ - rhs.z_; return rhs; }
        Vector operator/( Vector && rhs ) const { rhs.x_ = x_ / rhs.x_; rhs.y_ = y_ / rhs.y_; rhs.z_ = z_ / rhs.z_; return rhs; }
        Vector operator+( Real const & rhs ) const { Vector result( *this ); result += rhs; return result; }
        Vector operator*( Real const & rhs ) const { Vector result( *this ); result *= rhs; return result; }
        Vector operator-( Real const & rhs ) const { Vector result( *this ); result -= rhs; return result; }
        Vector operator/( Real const & rhs ) const { Vector result( *this ); result /= rhs; return result; }

        Vector abs() const { Vector result(  x_ < 0. ? -x_ : x_, y_ < 0. ? -y_ : y_, z_ < 0. ? -z_ : z_ ); return result; }
        Real len() const { return sqrt( lenSquared() ); }
        Real sum() const { return x_ + y_ + z_; }
        Real lenSquared() const { return x_ * x_ + y_ * y_ + z_ * z_; }
        Vector const & makeUnitVector( ) { operator/=( len() ); return *this; }
        void limitMax( Real const & limit = 1. ) {
            x_ = x_ > limit ? limit : x_;
            y_ = y_ > limit ? limit : y_;
            z_ = z_ > limit ? limit : z_;
        }
        void limitMin( Real const & limit = 0. ) {
            x_ = x_ < limit ? limit : x_;
            y_ = y_ < limit ? limit : y_;
            z_ = z_ < limit ? limit : z_;
        }

    private:
        Real x_;
        Real y_;
        Real z_;
    };

    Real dot( Vector const & a, Vector const & b ){ return ( a * b ).sum(); }
    Real dot( Vector const & a, Vector && b ){ return ( b *= a ).sum(); }
    Real dot( Vector && a, Vector const & b ){ return ( a *= b ).sum(); }
    Vector const cross( Vector const & a, Vector const & b ){
        return Vector{ a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(), a.x() * b.y() - a.y() * b.z() };
    }

    Vector const operator+( Real const & lhs, Vector const & rhs ){ return rhs + lhs; }
    Vector const operator*( Real const & lhs, Vector const & rhs ){ return rhs * lhs; }
    Vector const operator-( Real const & lhs, Vector const & rhs ){ return rhs - lhs; }
    Vector const operator/( Real const & lhs, Vector const & rhs ){ return rhs / lhs; }
    Vector const & operator+( Real const & lhs, Vector && rhs ){ return rhs += lhs; }
    Vector const & operator*( Real const & lhs, Vector && rhs ){ return rhs *= lhs; }
    Vector const & operator-( Real const & lhs, Vector && rhs ){ rhs.x() = lhs - rhs.x(); rhs.y() = lhs - rhs.y(); rhs.z() = lhs - rhs.z(); return rhs; }
    Vector const & operator/( Real const & lhs, Vector && rhs ){ rhs.x() = lhs / rhs.x(); rhs.y() = lhs / rhs.y(); rhs.z() = lhs / rhs.z(); return rhs; }

    static Vector const unit{1.,1.,1.};
    static Vector const zero{0.,0.,0.};
}


#endif //ZTRACER_VECTOR_H
