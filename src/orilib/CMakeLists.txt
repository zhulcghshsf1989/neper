cmake_minimum_required(VERSION 2.6)
project(orilib)

set (ORILIB_VERSION_MAJOR 2)
set (ORILIB_VERSION_MINOR 1)
set (ORILIB_VERSION_PATCH 1)
set (ORILIB_VERSION_DEVEL 2)

# should we use the optional libraries?
option (HAVE_GSL      "Use the GNU Scientific Library (GSL)" ON)
option (HAVE_PNG      "Use the libpng library"               OFF)
option (HAVE_LIBMATHEVAL "Use libmatheval"                   ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/ol_config.h.in"
  "${PROJECT_BINARY_DIR}/ol_config.h"
)

include_directories("${PROJECT_BINARY_DIR}")
# include_directories("${PROJECT_BINARY_DIR}/ut")
# include_directories("${PROJECT_BINARY_DIR}/ut")

add_definitions(-Wall -W -O3)

add_library (orilib 
ol_cal/ol_com.c
ol_cal/ol_csys.c
ol_cal/ol_dis.c
ol_cal/ol_inv.c
ol_cal/ol_sym.c
ol_des/ol_e.c
ol_des/ol_g.c
ol_des/ol_m.c
ol_des/ol_pole.c
ol_des/ol_q.c
ol_des/ol_R.c
ol_des/ol_rtheta.c
ol_gen/ol_nb_e.c
ol_gen/ol_nb_rtheta.c
ol_map/ol_map_alloc.c
ol_map/ol_map_copy.c
ol_map/ol_map_filter.c
ol_map/ol_map_png.c
ol_map/ol_map_rgb.c
ol_map/ol_map_transform.c
ol_set/ol_mean.c
ol_set/ol_set_alloc.c
ol_set/ol_set_aniso.c
ol_set/ol_set_sample.c
ol_set/ol_spread.c
)

## Link libraries
# this is for the GSL
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake
			${CMAKE_MODULE_PATH})

find_library (MATHLIB m)
if (MATHLIB)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${MATHLIB})
endif ()

if (HAVE_GSL)
  find_package(GSL REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${GSL_LIBRARIES})
endif ()

if (HAVE_PNG)
  find_package(PNG REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${PNG_LIBRARY})
endif ()

target_link_libraries (orilib ${EXTRA_LIBS})
