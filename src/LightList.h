/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZTRACE_LIGHTLIST_H
#define ZTRACE_LIGHTLIST_H

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
        void add( LightList const & newItems ) {
            for( auto item: newItems ) {
                lights_.push_back( item );
            }
        }

        LightListType::const_iterator begin() const { return lights_.begin(); }
        LightListType::const_iterator end() const { return lights_.end(); }
        Size size() { return lights_.size(); }

    private:
        LightListType lights_;
    };
}

#endif //ZTRACE_TRACEABLELIST_H
