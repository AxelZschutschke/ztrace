set(CMAKE_MACOSX_RPATH 1)

if(DEFINED CMAKE_BUILD_TYPE)
       set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of
       build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug
       Release RelWithDebInfo MinSizeRel.")
   else()
          set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build,
          options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release
          RelWithDebInfo MinSizeRel.")
endif()

cmake_minimum_required (VERSION 3.5.2)
project( ztracer )

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
  cmake_policy(SET CMP0042 NEW)
  cmake_policy(SET CMP0037 OLD)
  cmake_policy(SET CMP0038 OLD)
  cmake_policy(SET CMP0022 OLD)
else()
  # fixes CMake Version 3.5.2 on linux (otherwise it optimizes unused symbols away aka our equations)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic") 
  set(CMAKE_EXE_LINKER_FLAGS "-Wl,--no-as-needed -Wl,--add-needed") 
endif()

set (ztracer_VERSION_MAJOR 0)
set (ztracer_VERSION_MINOR 1)

add_definitions( -std=c++14 -I${CMAKE_CURRENT_SOURCE_DIR}/src)

#add_library( ztrace SHARED
#    src/ppmWriter.cpp
#        src/fileHandler.h)

add_executable( ztracer.x
    src/ztracer.cpp
    )
#target_link_libraries( ztracer.x rpg )


enable_testing()
include( CTest )

set( TESTS
    Component
    Colour
    Image
    Vector
    Ray
    Sphere
    )

foreach( TEST ${TESTS} )
    set( TESTNAME  test_${TEST})
    set( TESTSOURCE test/test_${TEST}.cpp )
    set( TESTEXE   ${TESTNAME}.x)

    add_executable( ${TESTEXE} ${TESTSOURCE} )
    target_link_libraries( ${TESTEXE} gtest )
    add_test( NAME ${TESTNAME} COMMAND ./${TESTEXE} )
endforeach()
