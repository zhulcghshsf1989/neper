cmake_minimum_required(VERSION 2.6)
project(neper_t)

option (HAVE_GSL         "Use the GNU Scientific Library (GSL)" ON )
option (HAVE_LIBMATHEVAL "Use libmatheval"                      ON )
option (HAVE_LIBSCOTCH   "Use libscotch"                        OFF)

include_directories("${PROJECT_BINARY_DIR}")

add_definitions(-Wall -W -O3)

add_library (neper_t
./IntePara/IntePara2.c
./IntePara/IntePara1.c
./Tess/Tess2.c
./Tess/Tess4.c
./Tess/Tess5.c
./Tess/Tess1.c
./Tess/Tess6.c
./Tess/Tess3.c
./InputData_t/InputData_t1.c
./InputData_t/InputData_t2.c
./InputData_t/InputData_t3.c
./InputData_t/InputData_t4.c
./Res_t/Res_t1.c
./Res_t/PointPoly/PointPoly1.c
./Res_t/net_oin/net_oin1.c
./Res_t/Connectivity/Connectivity1.c
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
./GermDistrib/GermDistrib1.c
./net_tess_sort/net_tess_sort1.c
./DomainComp/DomainComp1.c
./DomainComp/DomainComp2.c
./DomainComp/CubeDomain/CubeDomain2.c
./DomainComp/CubeDomain/CubeDomain1.c
./DomainComp/CubeDomain/CubeDomain3.c
./neper_t.c
./net_centroid/net_centroid1.c
./Miscellaneous/Miscellaneous2.c
./Miscellaneous/Miscellaneous1.c
./Miscellaneous/Miscellaneous3.c
)

set (EXTRA_LIBS ${EXTRA_LIBS} ut)
set (EXTRA_LIBS ${EXTRA_LIBS} neut)
set (EXTRA_LIBS ${EXTRA_LIBS} orilib)
set (EXTRA_LIBS ${EXTRA_LIBS} neper_fm)

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
