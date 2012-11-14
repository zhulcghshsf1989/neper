cmake_minimum_required(VERSION 2.6)
project(neper_vs)

option (HAVE_GSL         "Use the GNU Scientific Library (GSL)" ON )
option (HAVE_LIBMATHEVAL "Use libmatheval"                      ON )
option (HAVE_LIBSCOTCH   "Use libscotch"                        OFF)

include_directories("${PROJECT_BINARY_DIR}")

add_definitions(-Wall -W -O3)

add_library (neper_vs
./nevs_show/nevs_show2.c
./nevs_show/nevs_show1.c
./nevs_slice/nevs_slice1.c
./nevs_include/nevs_include1.c
./nevs_image/nevs_image1.c
./neper_vs.c
./nevs_processimage/nevs_processimage1.c
./nevs_load/nevs_load1.c
./nevs_data/nevs_data1.c
./nevs_data/nevs_geodata/nevs_geodata1.c
./nevs_data/nevs_meshdata/nevs_meshdata2.c
./nevs_data/nevs_meshdata/nevs_meshdata1.c
./nevs_data/nevs_data_utils/nevs_data_utils1.c
./nevs_camera/nevs_camera2.c
./nevs_camera/nevs_camera1.c
./nevs_print/nevs_print_utils/nevs_print_utils1.c
./nevs_print/nevs_print1.c
./nevs_print/nevs_print_foot/nevs_print_foot1.c
./nevs_print/nevs_print_header/nevs_print_header1.c
./nevs_print/nevs_print_mesh/nevs_print_mesh2.c
./nevs_print/nevs_print_mesh/nevs_print_mesh1.c
./nevs_print/nevs_print_tess/nevs_print_tess1.c
./nevs_print/nevs_print2.c
./nevs_scale/nevs_scale_print/nevs_scale_print1.c
./nevs_scale/nevs_scale1.c
)

set (EXTRA_LIBS ${EXTRA_LIBS} ut)
set (EXTRA_LIBS ${EXTRA_LIBS} neut)
set (EXTRA_LIBS ${EXTRA_LIBS} orilib)
  
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_library (MATHLIB m)
if (MATHLIB)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${MATHLIB})
endif ()

if (HAVE_GSL)
  find_package(GSL REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${GSL_LIBRARIES})
  include_directories(${GSL_INCLUDE_DIRS})
endif ()

if (HAVE_LIBSCOTCH)
  find_package (LIBSCOTCH REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${Scotch_LIBRARIES})
  include_directories (${Scotch_INCLUDE_DIRS})
endif ()

target_link_libraries (neper_vs ${EXTRA_LIBS})
