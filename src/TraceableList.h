//
// Created by Axel Zschutschke on 10/9/16.
//

#ifndef ZTRACER_TRACEABLELIST_H
#define ZTRACER_TRACEABLELIST_H

#include <memory>
#include <vector>
#include "Traceable.h"

namespace ztrace {

    class TraceableList : public Traceable {
    public:
        using SharedTraceable = std::shared_ptr<Traceable>;
        using Vector = std::vector<SharedTraceable>;

        TraceableList()
                : traceables_()
        {}

        void add( SharedTraceable const & newItem ) {
            traceables_.push_back( newItem );
        }

        Vector::iterator begin() { return traceables_.begin(); }
        Vector::iterator end() { return traceables_.end(); }

        bool hit( Ray const & ray, Real const & lowerLimit, Real const & upperLimit, TraceData & traceData ) const {
            bool hitSomething = false;
            TraceData traceData_temporary;
            for( auto&& item: traceables_ ) {
                bool hit_item = item->hit( ray, lowerLimit, upperLimit, traceData_temporary );
                // decide whether this is the nearest hit
                if( hit_item ) {
                    if( ! hitSomething ) {
                        hitSomething = true;
                        traceData = traceData_temporary;
                    } else {
                        if( traceData_temporary.positionOnRay < traceData.positionOnRay ) {
                            traceData = traceData_temporary;
                        }
                    }
                }
            }
            return  hitSomething;
        }

    private:
        Vector traceables_;
    };
}

#endif //ZTRACER_TRACEABLELIST_H
