cmake_minimum_required(VERSION 2.6)
project(neper_mm C)

option (HAVE_GSL         "Use the GNU Scientific Library (GSL)" ON )
option (HAVE_LIBMATHEVAL "Use libmatheval"                      ON )
option (HAVE_LIBSCOTCH   "Use libscotch"                        OFF)

include_directories("${PROJECT_BINARY_DIR}")

add_definitions(-Wall -W -g -O3)
set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})

add_library (neper_mm
./NSite/NSite4.c
./NSite/NSite3.c
./NSite/NSite2.c
./NSite/NSite1.c
./Res_mm/Res_mm1.c
./Res_mm/Res_mm2.c
./Res_mm/Res_mm3.c
./Res_mm/Res_mm4.c
./neper_mm.c
./RMeshing/RMeshing2.c
./RMeshing/RMeshingFNId/RMeshingFNId1.c
./RMeshing/RMeshingFNId/RMeshingFNId2.c
./RMeshing/RMeshingFGeo/RMeshingFGeo2.c
./RMeshing/RMeshingFGeo/RMeshingFGeo1.c
./RMeshing/RMeshing1.c
./InputData_mm/InputData_mm2.c
./InputData_mm/InputData_mm3.c
./InputData_mm/InputData_mm1.c
)

set (EXTRA_LIBS ${EXTRA_LIBS} ut)
set (EXTRA_LIBS ${EXTRA_LIBS} neut)
set (EXTRA_LIBS ${EXTRA_LIBS} neper_t)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_library (MATHLIB m)
if (MATHLIB)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${MATHLIB})
endif ()

if (HAVE_GSL)
  find_package(GSL REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${GSL_LIBRARIES})
  include_directories (${GSL_INCLUDE_DIRS})
endif ()

if (HAVE_LIBSCOTCH)
  find_package (LIBSCOTCH REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${Scotch_LIBRARIES})
  include_directories (${Scotch_INCLUDE_DIRS})
endif ()

target_link_libraries (neper_mm ${EXTRA_LIBS})
