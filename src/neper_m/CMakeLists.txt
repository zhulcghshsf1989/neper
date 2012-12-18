cmake_minimum_required(VERSION 2.6)
project(neper_m C)

option (HAVE_GSL         "Use the GNU Scientific Library (GSL)" ON )
option (HAVE_LIBMATHEVAL "Use libmatheval"                      ON )
option (HAVE_LIBSCOTCH   "Use libscotch"                        OFF)

include_directories("${PROJECT_BINARY_DIR}")

add_definitions(-Wall -W -g -O3)
set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})

add_library (neper_m
./nem_stat/nem_stat1.c
./nem_stat/nem_stat2.c
./nem_readmesh/nem_readmesh1.c
./nem_meshing/nem_meshing1.c
./nem_meshing/nem_meshing2.c
./nem_meshing/nem_meshing3.c
./nem_meshing/nem_meshing_prep/nem_meshing_prep1.c
./nem_meshing/nem_meshing_prep/nem_meshing_prep2.c
./nem_meshing/nem_meshing_0D/nem_meshing_0D2.c
./nem_meshing/nem_meshing_0D/nem_meshing_0D1.c
./nem_meshing/nem_meshing_1D/nem_meshing_1D2.c
./nem_meshing/nem_meshing_1D/nem_meshing_1D1.c
./nem_meshing/nem_meshing_1D/nem_meshing_1D3.c
./nem_meshing/nem_meshing_2D/nem_meshing_2D1.c
./nem_meshing/nem_meshing_3D/nem_meshing_3D1.c
./nem_meshing/nem_meshing_3D/nem_meshing_3D2.c
./nem_cleaning/nem_cleaning1.c
./nem_input/nem_input1.c
./nem_input/nem_input2.c
./nem_input/nem_input3.c
./nem/nem_mesh_stellar/nem_mesh_stellar1.c
./nem/nem_tess/nem_tess1.c
./nem/nem_mesh_gmsh/nem_mesh_gmsh3.c
./nem/nem_mesh_gmsh/nem_mesh_gmsh2.c
./nem/nem_mesh_gmsh/nem_mesh_gmsh1.c
./nem_init/nem_init1.c
./nem_init/nem_init2.c
./nem_transport/nem_transport1.c
./nem_transport/nem_transport2.c
./nem_partition/nem_partition5.c
./nem_partition/nem_partition1.c
./nem_partition/nem_partition4.c
./nem_partition/nem_partition3.c
./nem_partition/nem_partition2.c
./nem_reconmesh/nem_reconmesh1.c
./nem_reconmesh/nem_reconmesh2.c
./nem_writemesh/nem_writemesh1.c
./nem_nsets/nem_nsets1.c
./nem_nsets/nem_nsets2.c
./nem_meshing_hex/nem_meshing_hex1.c
./nem_meshing_hex/nem_meshing_hex2.c
./neper_m.c
)

set (EXTRA_LIBS ${EXTRA_LIBS} ut)
set (EXTRA_LIBS ${EXTRA_LIBS} neut)
  
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

target_link_libraries (neper_m ${EXTRA_LIBS})
