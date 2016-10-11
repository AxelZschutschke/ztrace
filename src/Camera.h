//
// Created by Axel Zschutschke on 10/9/16.
//

#ifndef ZTRACER_CAMERA_H
#define ZTRACER_CAMERA_H

#include "Ray.h"
#include "Vector.h"

namespace ztrace {


    class Camera {
    public:
        Camera()
                : position_()
                , pointingDirection_(0.,0.,-1.)
                , pointingNormalX_(1.,0.,0.)
                , pointingNormalY_(0.,1.,0.)
                , width_(1.6)
                , height_(0.9)
                , focalLength_(1.0)
                , aperture_(0.01)
        {}

        Camera( Vector const & position, Vector const & pointingDirection, Real const & width = 1.6, Real const & height = 0.9, Real const & focalLength = 1.0, Real const & aperture = 0.05 )
                : position_(position)
                , pointingDirection_(pointingDirection)
                , pointingNormalX_()
                , pointingNormalY_()
                , width_(width)
                , height_(height)
                , focalLength_(focalLength)
                , aperture_(aperture)
        {
            std::cout << aperture_ << std::endl;
            updateState();
        }

        void updateState() {
            // earths normal (to level the camera)
            static Vector earthNormal( 0., -1., 0. );
            pointingDirection_.makeUnitVector();
            pointingNormalX_ = -cross( pointingDirection_, earthNormal );
            pointingNormalY_ = -cross( pointingDirection_, pointingNormalX_);
            pointingNormalX_.makeUnitVector();
            pointingNormalY_.makeUnitVector();
        }

        Ray const emitRay( Real const & screenX, Real const & screenY ) const {
            Vector offset_ = randomOffsetAperture();
            Vector target_ = pointingDirection_ * focalLength_; 
            target_ += pointingNormalX_ * width_ * (screenX - 0.5 );
            target_ += pointingNormalY_ * height_ * (0.5 - screenY );
            return Ray{ position_ + offset_, target_ - offset_ };
        }

    private:
        Vector const randomOffsetAperture( ) const {
            Vector offset_;
            static Vector unit(1.,1.,1.);
            do {
                offset_ = 2. * ( drand48() * pointingNormalX_ + drand48() * pointingNormalY_ ) - pointingNormalX_ - pointingNormalY_;
            } while( offset_.len() > 1. );
            return offset_ * aperture_;
        }

        Vector position_;
        Vector pointingDirection_;
        Vector pointingNormalX_;
        Vector pointingNormalY_;
        Real width_;    // how much fisheye it is
        Real height_;
        Real focalLength_;
        Real aperture_;
    };
}

#endif //ZTRACER_CAMERA_H
