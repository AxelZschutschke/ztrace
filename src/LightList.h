//
// Created by Axel Zschutschke on 10/9/16.
//

#ifndef ZTRACER_LIGHTLIST_H
#define ZTRACER_LIGHTLIST_H

#include <memory>
#include <vector>
#include "Light.h"

namespace ztrace {

    class LightList {
    public:
        using SharedLight = std::shared_ptr<Light>;
        using Vector = std::vector<SharedLight>;

        LightList()
                : lights_()
        {}

        void add( SharedLight const & newItem ) {
            lights_.push_back( newItem );
        }

        Vector::const_iterator begin() const { return lights_.begin(); }
        Vector::const_iterator end() const { return lights_.end(); }

    private:
        Vector lights_;
    };
}

#endif //ZTRACER_TRACEABLELIST_H
