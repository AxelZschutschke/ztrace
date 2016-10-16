#ifndef ZTRACE_RANDOM_H
#define ZTRACE_RANDOM_H

// pure mock for testing purposes 
// "interfaces" to stdlib::drand48

#ifdef ZTRACE_TESTING
#include "Types.h"
namespace ztrace {
    Real random() { return 0.5; }
}
#else
#include <cstdlib>
namespace ztrace {
    double random() { return drand48(); }
}
#endif

#endif
