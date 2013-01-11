cmake_minimum_required(VERSION 2.6)
project(neper_t C)

option (HAVE_GSL         "Use the GNU Scientific Library (GSL)" ON )
option (HAVE_LIBMATHEVAL "Use libmatheval"                      ON )
option (HAVE_LIBSCOTCH   "Use libscotch"                        OFF)

include_directories("${PROJECT_BINARY_DIR}")

add_definitions(-Wall -W -g -O3)
set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})

add_library (neper_t
./net_init/net_init1.c
./net_regularization/MergeTess/MergeTess2.c
./net_regularization/MergeTess/MergeTess1.c
./net_regularization/MergeTess/DeleteEdge/DeleteEdge1.c
./net_regularization/MergeTess/DeleteEdge/DeleteEdge2.c
./net_regularization/MergeTess/DeleteEdge/FaceFF/FaceFFPlanes2.c
./net_regularization/MergeTess/DeleteEdge/FaceFF/FaceFFPlanes1.c
./net_regularization/MergeTess/DeleteEdge/FaceFF/FaceFF1.c
./net_regularization/MergeTess/DeleteEdge/DeleteEdge3.c
./net_regularization/MergeTess/DeleteEdge/DeleteEdge5.c
./net_regularization/MergeTess/DeleteEdge/DeleteEdge4.c
./net_regularization/net_regularization1.c
./net_vox/net_vox1.c
./net_vox/net_vox2.c
./net_utils/net_utils1.c
./net_tess/net_tess.c
./net_tess/net_tesl/net_tesl2.c
./net_tess/net_tesl/net_tesl4.c
./net_tess/net_tesl/net_tesl5.c
./net_tess/net_tesl/net_tesl1.c
./net_tess/net_tesl/net_tesl6.c
./net_tess/net_tesl/net_tesl3.c
./net_tess/net_polycomp/PolyDet/PolyDet.c
./net_tess/net_polycomp/PolyDet/PolyCalc/PolyCalc5.c
./net_tess/net_polycomp/PolyDet/PolyCalc/PolyCalc4.c
./net_tess/net_polycomp/PolyDet/PolyCalc/PolyCalc6.c
./net_tess/net_polycomp/PolyDet/PolyCalc/PolyCalc3.c
./net_tess/net_polycomp/PolyDet/PolyCalc/PolyCalc1.c
./net_tess/net_polycomp/PolyDet/PolyCalc/PolyCalc2.c
./net_tess/net_polycomp/PolyAlloc/PolyAlloc.c
./net_tess/net_polycomp/PolyFree/PolyFree.c
./net_tess/net_polycomp/PolyReg/PolyReg2.c
./net_tess/net_polycomp/PolyReg/PolyReg1.c
./net_tess/net_polycomp/PolyReg/PolyReg3.c
./net_tess/net_polycomp/net_polycomp.c
./net_tess/net_centroid/net_centroid1.c
./net_input/net_input1.c
./net_input/net_input2.c
./net_input/net_input3.c
./net_res/net_res1.c
./net_res/PointPoly/PointPoly1.c
./net_res/net_stat/net_stat1.c
./net_res/net_stat/net_stat2.c
./net_germ/RegDistrib/RegDistrib1.c
./net_germ/RandDistrib/RandDistrib2.c
./net_germ/RandDistrib/RandDistrib1.c
./net_germ/DefGermSet/DefGermSet1.c
./net_germ/DefGermSet/DefGermSet2.c
./net_germ/DefGermSet/DefGermSet3.c
./net_germ/net_germ1.c
./net_germ/net_germ2.c
./net_domain/net_domain1.c
./net_domain/net_domain2.c
./net_domain/net_domain_cube/net_domain_cube2.c
./net_domain/net_domain_cube/net_domain_cube1.c
./net_domain/net_domain_cube/net_domain_cube3.c
./neper_t.c
)

set (EXTRA_LIBS ${EXTRA_LIBS} ut)
set (EXTRA_LIBS ${EXTRA_LIBS} neut)
set (EXTRA_LIBS ${EXTRA_LIBS} orilib)
set (EXTRA_LIBS ${EXTRA_LIBS} neper_m)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_library (MATHLIB m)
if (MATHLIB)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${MATHLIB})
endif ()

if (HAVE_LIBMATHEVAL)
  find_package (LIBMATHEVAL REQUIRED)
  if (Matheval_LIBRARIES)
    set (EXTRA_LIBS ${EXTRA_LIBS} ${Matheval_LIBRARIES})
    include_directories (${Matheval_INCLUDE_DIRS})
  endif ()
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

target_link_libraries (neper_t ${EXTRA_LIBS})
