cmake_minimum_required(VERSION 2.6)
project(neut C)

# should we use the optional libraries?
option (HAVE_GSL       "Use the GNU Scientific Library (GSL)" ON)
option (HAVE_LIBSCOTCH   "Use libscotch"                        OFF)

include_directories("${PROJECT_BINARY_DIR}")

add_definitions(-Wall -W -g -O3)

add_library (neut 
./neut_tess/neut_tess.c
./neut_tess/neut_tess_deform/neut_tess_deform1.c
./neut_tess/neut_tess_geo/neut_tess_geo3.c
./neut_tess/neut_tess_geo/neut_tess_geo2.c
./neut_tess/neut_tess_geo/neut_tess_geo1.c
./neut_poly/neut_poly.c
./neut_poly/neut_poly_polymod/neut_poly_polymod1.c
./neut_poly/neut_poly_polymod/neut_poly_polymod2.c
./neut_poly/neut_poly_polymod/neut_poly_polymod3.c
./neut_data/neut_data.c
./neut_part/neut_part.c
./neut_multim/neut_multim.c
./neut_elt/neut_elt.c
./neut_mesh/neut_nodes.c
./neut_mesh/fscanf_msh/fscanf_msh3.c
./neut_mesh/fscanf_msh/fscanf_msh2.c
./neut_mesh/fscanf_msh/fscanf_msh4.c
./neut_mesh/fscanf_msh/fscanf_msh1.c
./neut_mesh/neut_mesh_renumber/neut_mesh_renumber3.c
./neut_mesh/neut_mesh_renumber/neut_mesh_renumber1.c
./neut_mesh/neut_mesh_renumber/neut_mesh_renumber2.c
./neut_mesh/neut_mesh.c
./neut_scale/neut_scale.c
./neut_rand/neut_rand.c
./neut_gmsh/neut_gmsh1.c
./neut_debug/neut_debug.c
./neut_germset/neut_germset.c
./neut_utils/neut_utils.c
./neut_povray/neut_povray.c
./neut_nset/neut_nset.c
./neut_mmesh/neut_mmesh.c
./neut_print/neut_print.c
./neut_geo/neut_geo_compress/neut_geo_compress1.c
./neut_geo/neut_geo_compress/neut_geo_compress2.c
./neut_geo/neut_geo_compress/neut_geo_compress3.c
./neut_geo/neut_geo_fprintf/neut_geo_fprintf2.c
./neut_geo/neut_geo_fprintf/neut_geo_fprintf1.c
./neut_geo/neut_geo_geo/neut_geo_geo3.c
./neut_geo/neut_geo_geo/neut_geo_geo2.c
./neut_geo/neut_geo_geo/neut_geo_geo1.c
./neut_geo/neut_geo_fscanf/neut_geo_fscanf2.c
./neut_geo/neut_geo_fscanf/neut_geo_fscanf1.c
./neut_geo/neut_geo_fscanf/neut_geo_fscanf_1p9/neut_geo_fscanf2_1p9.c
./neut_geo/neut_geo_fscanf/neut_geo_fscanf_1p9/neut_geo_fscanf1_1p9.c
./neut_geo/neut_geo_op/neut_geo_op1.c
./neut_geo/neut_geo_op/neut_geo_op2.c
./neut_geo/neut_geo_test/neut_geo_test1.c
./neut_geo/neut_geo_test/TestVer/TestVer1.c
./neut_geo/neut_geo_test/TestVer/TestVer2.c
./neut_geo/neut_geo_test/TestFace/TestFace1.c
./neut_geo/neut_geo_test/TestFace/TestFace2.c
./neut_geo/neut_geo_test/TestEdge/TestEdge1.c
./neut_geo/neut_geo_test/TestEdge/TestEdge2.c
./neut_geo/neut_geo_test/TestPoly/TestPoly2.c
./neut_geo/neut_geo_test/TestPoly/TestPoly1.c
./neut_geo/neut_geo_test/TestDom/TestDom1.c
./neut_geo/neut_geo_test/TestDom/TestDom2.c
./neut_geo/neut_geo_gen/neut_geo_gen.c
)

## Link libraries

set (EXTRA_LIBS ${EXTRA_LIBS} ut)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake
			${CMAKE_MODULE_PATH})

find_library (MATHLIB m)
if (MATHLIB)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${MATHLIB})
endif ()

if (HAVE_GSL)
  find_package(GSL REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${GSL_LIBRARIES})
endif (HAVE_GSL)

if (HAVE_LIBSCOTCH)
  find_package (LIBSCOTCH REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${Scotch_LIBRARIES})
  include_directories (${Scotch_INCLUDE_DIRS})
endif ()

target_link_libraries (neut ${EXTRA_LIBS})
