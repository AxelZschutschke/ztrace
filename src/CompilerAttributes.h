#ifndef COMPILER_ATTRIBUTES
#define COMPILER_ATTRIBUTES

#if defined(__GNUC__) && (__GNUC__ >= 4)  || defined(__clang__) || defined(__ICC) || defined(__INTEL_COMPILER)  
#define ATT_CHECK_RESULT __attribute__ ((warn_unused_result))
#define ATT_UNUSED __attribute__((unused))
#elif defined(_MSC_VER) && (_MSC_VER >= 1700)
#define ATT_CHECK_RESULT _Check_return_
#define ATT_UNUSED
#else
#define ATT_CHECK_RESULT
#define ATT_UNUSED
#endif

#endif