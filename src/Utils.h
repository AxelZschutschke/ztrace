#ifndef ZTRACE_UTILS_H
#define ZTRACE_UTILS_H

#ifndef ZTRACE_TESTING
#include <stdlib.h>
#endif

#include "Types.h"
#include "Vector.h"

namespace ztrace {
        Vector const randomScatter( ) {
          Vector result;
          do {
              result = 2. * Vector{drand48(), drand48(), drand48()} - unit;
          } while( result.len() > 1. );

          return Vector{ result };
        }
        Vector const reflect( Vector const & in, Vector const & normal ) {
            return Vector{ in - 2 * dot( in, normal ) * normal };
        }

}

#endif
