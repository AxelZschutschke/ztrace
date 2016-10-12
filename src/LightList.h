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
        using LightListType = std::vector<SharedLight>;

        LightList()
                : lights_()
        {}

        void add( SharedLight const & newItem ) {
            lights_.push_back( newItem );
        }

        LightListType::const_iterator begin() const { return lights_.begin(); }
        LightListType::const_iterator end() const { return lights_.end(); }
        Size size() { return lights_.size(); }

    private:
        LightListType lights_;
    };
}

#endif //ZTRACER_TRACEABLELIST_H
