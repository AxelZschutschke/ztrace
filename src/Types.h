//
// Created by Axel Zschutschke on 10/8/16.
//

#ifndef ZTRACER_TYPES_H
#define ZTRACER_TYPES_H

#include <string>

namespace ztrace
{
    using Real = float;
    using Int  = long;
    using Size = size_t;
    using String = std::string;

    template<typename Target, typename Source>
    Target convert( Source const & s );
}

#endif //ZTRACER_TYPES_H
