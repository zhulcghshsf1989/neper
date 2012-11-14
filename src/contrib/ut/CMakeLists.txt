cmake_minimum_required(VERSION 2.6)
project(ut C)
# The version number.
set (UT_VERSION_MAJOR 1)
set (UT_VERSION_MINOR 0)
set (UT_VERSION_PATCH 0)

# should we use the optional libraries?
option (HAVE_GSL      "Use the GNU Scientific Library (GSL)" ON)
option (HAVE_LIBMATHEVAL "Use libmatheval"                   ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/ut_config.h.in"
  "${PROJECT_BINARY_DIR}/ut_config.h"
)

include_directories("${PROJECT_BINARY_DIR}")

add_definitions(-Wall -W -O3)

add_library (ut 
  ut_alloc/ut_alloc.c
  ut_arg/ut_arg.c
  ut_array/ut_array.c
  ut_color/ut_color.c
  ut_fig/ut_fig.c
  ut_file/ut_file.c
  ut_free/ut_free.c
  ut_math/ut_math.c
  ut_mat/ut_mat.c
  ut_num/ut_num.c
  ut_legacy/ut_legacy.c
  ut_option/ut_option.c
  ut_print/ut_print.c
  ut_space/ut_space.c
  ut_string/ut_string.c
  ut_sys/ut_sys.c
  ut_tensor/ut_tensor.c
  ut_time/ut_time.c
  ut_vector/ut_vector.c
)

## Link libraries

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_library (MATHLIB m)
if (MATHLIB)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${MATHLIB})
endif ()

if (HAVE_GSL)
  find_package(GSL REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${GSL_LIBRARIES})
  include_directories (${GSL_INCLUDE_DIR})
endif (HAVE_GSL)

if (HAVE_LIBMATHEVAL)
    find_package(LIBMATHEVAL REQUIRED)
    include_directories( ${Matheval_INCLUDE_DIRS} )
    link_directories( ${Matheval_LIBRARY_DIRS} )
    set (EXTRA_LIBS ${EXTRA_LIBS} ${Matheval_LIBRARIES} )
endif(HAVE_LIBMATHEVAL)


target_link_libraries (ut ${EXTRA_LIBS})

# Installation of the library
# SET (CMAKE_INSTALL_PREFIX /usr/local/lib)

set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_alloc.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_arg.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_array.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_color.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_fig.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_file.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_free.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_math.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_mat.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_num.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_legacy.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_option.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_print.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_space.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_string.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_sys.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_tensor.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_time.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_vector.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_BINARY_DIR}/ut_config.h)

