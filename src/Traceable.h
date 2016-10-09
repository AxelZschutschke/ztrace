//
// Created by Axel Zschutschke on 10/9/16.
//

#ifndef ZTRACER_TRACEABLE_H
#define ZTRACER_TRACEABLE_H

#include "Ray.h"

namespace ztrace {

    class Material;

    struct TraceData {
        using MaterialPtr = std::shared_ptr<Material>;

        Real positionOnRay;
        Vector point;
        Vector normal;
        MaterialPtr material;
    };

    class Traceable {
    public:
        virtual bool hit( Ray const & ray, Real const & intervalLower, Real const & intervalUpper, TraceData & traceData ) const = 0;
    };
}

#endif //ZTRACER_TRACEABLE_H
