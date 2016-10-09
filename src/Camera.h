//
// Created by Axel Zschutschke on 10/9/16.
//

#ifndef ZTRACER_CAMERA_H
#define ZTRACER_CAMERA_H

#include "Ray.h"
#include "Vector.h"

namespace ztrace {

    // earths normal (to level the camera)
    static Vector earthNormal( 0., -1., 0. );

    class Camera {
    public:
        Camera()
                : position_()
                , pointingDirection_(0.,0.,-1.)
                , pointingNormalX_(1.,0.,0.)
                , pointingNormalY_(0.,1.,0.)
                , width_(1.6)
                , height_(0.9)
        {}

        Camera( Vector const & position, Vector const & pointingDirection, Real const & width = 1.6, Real const & height = 0.9 )
                : position_(position)
                , pointingDirection_(pointingDirection)
                , pointingNormalX_()
                , pointingNormalY_()
                , width_(width)
                , height_(height)
        {
            updateState();
        }

        void updateState() {
            pointingDirection_.makeUnitVector();
            pointingNormalX_ = cross( pointingDirection_, earthNormal );
            pointingNormalY_ = cross( pointingDirection_, pointingNormalX_);
            pointingNormalX_.makeUnitVector();
            pointingNormalY_.makeUnitVector();
            pointingNormalX_ *= width_;
            pointingNormalY_ *= height_;
        }

        Ray const emitRay( Real const & screenX, Real const & screenY ){
            return Ray{ position_, (pointingNormalX_ * ( screenX - 0.5 ) + pointingNormalY_ * ( 0.5 - screenY ) ) + pointingDirection_ };
        }

    private:
        Vector position_;
        Vector pointingDirection_;
        Vector pointingNormalX_;
        Vector pointingNormalY_;
        Real width_;    // how much fisheye it is
        Real height_;
    };
}

#endif //ZTRACER_CAMERA_H
