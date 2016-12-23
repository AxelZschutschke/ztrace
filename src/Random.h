/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZTRACE_RANDOM_H
#define ZTRACE_RANDOM_H

#include "CompilerAttributes.h"
// pure mock for testing purposes
// "interfaces" to stdlib::drand48

#ifdef ZTRACE_TESTING
#define ZTRACE_TESTING_RAND 0.5
#include "Types.h"
namespace ztrace
{
Real random() { return ZTRACE_TESTING_RAND; }
}
#else
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
namespace ztrace
{
	Real random() { return (Real) 0.5; }
	void randomInit(__int64 seed ATT_UNUSED) {}
}
#else
#include <cstdlib>
namespace ztrace
{
Real random() { return (Real) drand48(); }
void randomInit(long int seed) { srand48(seed); }
}
#endif
#endif
#endif
