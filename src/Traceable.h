//
// Created by Axel Zschutschke on 10/9/16.
//

#ifndef ZTRACER_TRACEABLE_H
#define ZTRACER_TRACEABLE_H

#include "Ray.h"

namespace ztrace {

    struct TraceData {
        Real positionOnRay;
        Vector point;
        Vector normal;
    };

    class Traceable {
    public:
        virtual bool hit( Ray const & ray, Real const & intervalLower, Real const & intervalUpper, TraceData & traceData ) = 0;
    };
}

#endif //ZTRACER_TRACEABLE_H
