//
// Created by Axel Zschutschke on 10/8/16.
//

#ifndef ZTRACER_RAY_H
#define ZTRACER_RAY_H

#include "Vector.h"

namespace ztrace {

    class Ray {
    public:
        Ray( )
                : A_( )
                , B_( )
        {}
        Ray( Vector const & A, Vector const & B )
                : A_(A)
                , B_(B)
        {}

        Vector const & origin() const { return A_; }
        Vector const & direction() const { return B_; }
        Vector const   positionLength( Real length ) const { return A_ + B_ * length; }

    private:
        Vector A_;
        Vector B_;
    };

    class ShadowRay : public Ray {
    public:
        ShadowRay( )
                : Ray()
                , specular_()
        {}
        ShadowRay( Vector const & A, Vector const & B, bool specular = false )
                : Ray( A, B )
                , specular_(specular)
        {}

        bool specular() { return specular_; }

    private:
        bool specular_;
    };
}

#endif //ZTRACER_RAY_H
