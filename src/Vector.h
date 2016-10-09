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
        using RealVector = std::vector<Real>;
        using RealVectorIterator = RealVector::iterator;
        using RealVectorConstIterator = RealVector::const_iterator;
    public:
        Vector()
                : vec_{0.,0.,0.}
                , x_(vec_[0])
                , y_(vec_[1])
                , z_(vec_[2])
        {}

        Vector( Real const & x, Real const & y, Real const & z )
                : vec_{x,y,z}
                , x_(vec_[0])
                , y_(vec_[1])
                , z_(vec_[2])
        {}
        Vector( Vector const & rhs )
                : vec_{rhs.vec_}
                , x_(vec_[0])
                , y_(vec_[1])
                , z_(vec_[2])
        {}
        Vector const & operator=( Vector const & rhs ){
            std::copy( rhs.begin(), rhs.end(), vec_.begin() );
            return *this;
        }

        friend ostream & operator<<( ostream & out, Vector const & vector ){
            out << vector.vec_[0] << " " << vector.vec_[1] << " " << vector.vec_[2];
            return out;
        }

        Real const & x() const { return x_; }
        Real const & y() const { return y_; }
        Real const & z() const { return z_; }
        RealVectorIterator begin() { return vec_.begin(); }
        RealVectorIterator end() { return vec_.end(); }
        RealVectorConstIterator begin() const { return vec_.begin(); }
        RealVectorConstIterator end() const  { return vec_.end(); }

        Vector const operator+( ) const { return *this; }
        Vector operator-( ) const { Vector result( *this ); result.x_ = -result.x_; result.y_ = -result.y_; result.z_ = -result.z_; return result; }
        Vector const & operator+=( Vector const & rhs ){ std::transform( begin(), end(), rhs.begin(), vec_.begin(), std::plus<Real>() ); return *this; }
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
        RealVector vec_;
        Real & x_;
        Real & y_;
        Real & z_;
    };

    Real const dot( Vector const & a, Vector const & b ){
        return ( a * b ).sum();
    }
    Vector const cross( Vector const & a, Vector const & b ){
        return Vector{ a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(), a.x() * b.y() - a.y() * b.z() };
    }

    Vector const operator+( Real const & lhs, Vector const & rhs ){ return rhs + lhs; }
    Vector const operator*( Real const & lhs, Vector const & rhs ){ return rhs * lhs; }
    Vector const operator-( Real const & lhs, Vector const & rhs ){ return rhs - lhs; }
    Vector const operator/( Real const & lhs, Vector const & rhs ){ return rhs / lhs; }
}


#endif //ZTRACER_VECTOR_H
