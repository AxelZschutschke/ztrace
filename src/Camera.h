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
                , pointingNormalX(1.,0.,0.)
                , pointingNormalY(0.,1.,0.)
                , width_(1.6)
                , height_(0.9)
        {}

        Camera( Vector const & position, Vector const & pointingDirection, Real const & width = 1.6, Real const & height = 0.9 )
                : position_(position)
                , pointingDirection_(pointingDirection)
                , pointingNormalX()
                , pointingNormalY()
                , width_(width)
                , height_(height)
        {
            updateState();
        }

        void updateState() {
            pointingDirection_.makeUnitVector();
            pointingNormalX_ = cross( pointingDirection_, earthNormal ).makeUnitVector();
            pointingNormalY_ = cross( pointingDirection_, pointingNormalX_).makeUnitVector();
            pointingNormalX_ *= width_;
            pointingNormalY_ *= height_;
        }

        Ray const emitRay( Real const & screenX, Real const & screenY ){
            return Ray{ position_, (pointingNormalX_ * screenX + pointingNormalY_ * screenY ) + pointingDirection_ };
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
