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
./RegularizeTess/MergeTess/MergeTess2.c
./RegularizeTess/MergeTess/MergeTess1.c
./RegularizeTess/MergeTess/DeleteEdge/DeleteEdge1.c
./RegularizeTess/MergeTess/DeleteEdge/DeleteEdge2.c
./RegularizeTess/MergeTess/DeleteEdge/FaceFF/FaceFFPlanes2.c
./RegularizeTess/MergeTess/DeleteEdge/FaceFF/FaceFFPlanes1.c
./RegularizeTess/MergeTess/DeleteEdge/FaceFF/FaceFF1.c
./RegularizeTess/MergeTess/DeleteEdge/DeleteEdge3.c
./RegularizeTess/MergeTess/DeleteEdge/DeleteEdge5.c
./RegularizeTess/MergeTess/DeleteEdge/DeleteEdge4.c
./RegularizeTess/RegularizeTess1.c
./net_vox/net_vox1.c
./net_vox/net_vox2.c
./net_utils/net_utils1.c
./Tesl/Tesl2.c
./Tesl/Tesl4.c
./Tesl/Tesl5.c
./Tesl/Tesl1.c
./Tesl/Tesl6.c
./Tesl/Tesl3.c
./InputData_t/InputData_t1.c
./InputData_t/InputData_t2.c
./InputData_t/InputData_t3.c
./Res_t/Res_t1.c
./Res_t/PointPoly/PointPoly1.c
./Res_t/net_stat/net_stat1.c
./Res_t/net_stat/net_stat2.c
./PolyComp/PolyDet/PolyDet.c
./PolyComp/PolyDet/PolyCalc/PolyCalc5.c
./PolyComp/PolyDet/PolyCalc/PolyCalc4.c
./PolyComp/PolyDet/PolyCalc/PolyCalc6.c
./PolyComp/PolyDet/PolyCalc/PolyCalc3.c
./PolyComp/PolyDet/PolyCalc/PolyCalc1.c
./PolyComp/PolyDet/PolyCalc/PolyCalc2.c
./PolyComp/PolyAlloc/PolyAlloc.c
./PolyComp/PolyFree/PolyFree.c
./PolyComp/PolyReg/PolyReg2.c
./PolyComp/PolyReg/PolyReg1.c
./PolyComp/PolyReg/PolyReg3.c
./PolyComp/PolyComp.c
./GermDistrib/RegDistrib/RegDistrib1.c
./GermDistrib/RandDistrib/RandDistrib2.c
./GermDistrib/RandDistrib/RandDistrib1.c
./GermDistrib/DefGermSet/DefGermSet1.c
./GermDistrib/DefGermSet/DefGermSet2.c
./GermDistrib/DefGermSet/DefGermSet3.c
./GermDistrib/GermDistrib1.c
./GermDistrib/GermDistrib2.c
./net_tess_sort/net_tess_sort1.c
./DomainComp/DomainComp1.c
./DomainComp/DomainComp2.c
./DomainComp/CubeDomain/CubeDomain2.c
./DomainComp/CubeDomain/CubeDomain1.c
./DomainComp/CubeDomain/CubeDomain3.c
./neper_t.c
./net_centroid/net_centroid1.c
./Miscellaneous/Miscellaneous1.c
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
