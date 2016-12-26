###############################################################################
# ZTRACER project preamble
###############################################################################
cmake_minimum_required (VERSION 3.5.2)
project( ztracer )
set (ztracer_VERSION_MAJOR 0)
set (ztracer_VERSION_MINOR 1)

if(DEFINED CMAKE_BUILD_TYPE)
       set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of
       build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug
       Release RelWithDebInfo MinSizeRel.")
   else()
       set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build,
       options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release
       RelWithDebInfo MinSizeRel.")
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
  set(CMAKE_MACOSX_RPATH 1)
  cmake_policy(SET CMP0042 NEW)
  cmake_policy(SET CMP0037 OLD)
  cmake_policy(SET CMP0038 OLD)
  cmake_policy(SET CMP0022 OLD)
  set(EXE_FILE_EXTENSION ".x" )
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(GOOGLE_TEST_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/../googletest/googletest/include/" CACHE STRING 
	"Location of googletest include directory" )
  set(GOOGLE_TEST_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../googletest/googletest/msvc/x64/Release/" CACHE STRING 
	"Location of googletest library" )
  include_directories( ${GOOGLE_TEST_INCLUDE} )
  link_directories( ${GOOGLE_TEST_LIB} )
  set(EXE_FILE_EXTENSION "" )
else()
  # fixes CMake Version 3.5.2 on linux
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic") 
  set(CMAKE_EXE_LINKER_FLAGS "-Wl,--no-as-needed -Wl,--add-needed") 
  set(EXE_FILE_EXTENSION ".x" )
endif()


add_definitions( -std=c++14 -I${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable( ztracer${EXE_FILE_EXTENSION}
    src/ztracer.cpp
    )

enable_testing()
include( CTest )

set( TESTS
    Gloss
    Material
    ColourConverter
    Image
    Vector
    Vector_Op_VecVec
    Vector_Op_VecReal
    Vector_Op_RealVec
    Ray
    ShadowRay
    Sphere
    TraceableList
    TraceableNode 
    Utils
    )

foreach( TEST ${TESTS} )
    set( TESTNAME  test_${TEST})
    set( TESTSOURCE test/test_${TEST}.cpp )
    set( TESTEXE   ${TESTNAME}${EXE_FILE_EXTENSION})

    add_executable( ${TESTEXE} ${TESTSOURCE} )
    target_link_libraries( ${TESTEXE} gtest )
    add_test( NAME ${TESTNAME} COMMAND ./${TESTEXE} )
endforeach()
