cmake_minimum_required(VERSION 2.6)
project(neut C)

# should we use the optional libraries?
option (HAVE_GSL       "Use the GNU Scientific Library (GSL)" ON)
option (HAVE_LIBSCOTCH   "Use libscotch"                        OFF)

include_directories("${PROJECT_BINARY_DIR}")

add_definitions(-Wall -W -g -O3)
set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})

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
./neut_nodes/neut_nodes.c
./neut_mesh/neut_mesh_fscanf/neut_mesh_fscanf_fepx/neut_mesh_fscanf_fepx1.c
./neut_mesh/neut_mesh_fscanf/neut_mesh_fscanf_stellar/neut_mesh_fscanf_stellar1.c
./neut_mesh/neut_mesh_fscanf/neut_mesh_fscanf_geof/neut_mesh_fscanf_geof1.c
./neut_mesh/neut_mesh_fscanf/neut_mesh_fscanf_gmsh/neut_mesh_fscanf_gmsh1.c
./neut_mesh/neut_mesh_fscanf/neut_mesh_fscanf_gmsh/neut_mesh_fscanf_gmsh2.c
./neut_mesh/neut_mesh_fscanf/neut_mesh_fscanf_gmsh/neut_mesh_fscanf_gmsh3.c
./neut_mesh/neut_mesh_fscanf/neut_mesh_fscanf_gmsh/neut_mesh_fscanf_gmsh4.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_gmsh/neut_mesh_fprintf_gmsh1.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_fepx/neut_mesh_fprintf_fepx1.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_fepx/neut_mesh_fprintf_fepx2.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_fepx/neut_mesh_fprintf_fepx3.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_geof/neut_mesh_fprintf_geof1.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_geof/neut_mesh_fprintf_geof2.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_geof/neut_mesh_fprintf_geof3.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_inp/neut_mesh_fprintf_inp1.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_inp/neut_mesh_fprintf_inp2.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_scotch/neut_mesh_fprintf_scotch1.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_stellar/neut_mesh_fprintf_stellar1.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_stellar/neut_mesh_fprintf_stellar2.c
./neut_mesh/neut_mesh_op/neut_mesh_op.c
./neut_mesh/neut_mesh_geom/neut_mesh_geom.c
./neut_mesh/neut_mesh_topo/neut_mesh_topo.c
./neut_mesh/neut_mesh_gen/neut_mesh_gen.c
./neut_mesh/neut_mesh_part/neut_mesh_part.c
./neut_mesh/neut_mesh_quad/neut_mesh_quad.c
./neut_scale/neut_scale.c
./neut_rand/neut_rand.c
./neut_gmsh/neut_gmsh1.c
./neut_debug/neut_debug.c
./neut_germset/neut_germset.c
./neut_povray/neut_povray.c
./neut_nset/neut_nset.c
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
./neut_geo/neut_geo_fscanf/neut_geo_fscanf_1p10/neut_geo_fscanf2_1p10.c
./neut_geo/neut_geo_fscanf/neut_geo_fscanf_1p10/neut_geo_fscanf1_1p10.c
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
./neut_vox/neut_vox_fscanf/neut_vox_fscanf1.c
./neut_vox/neut_vox_fscanf/neut_vox_fscanf2.c
./neut_vox/neut_vox_fprintf/neut_vox_fprintf1.c
./neut_vox/neut_vox_fprintf/neut_vox_fprintf2.c
./neut_vox/neut_vox_op/neut_vox_op1.c
./neut_vox/neut_vox_gen/neut_vox_gen1.c
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
  include_directories(${GSL_INCLUDE_DIRS})
endif (HAVE_GSL)

if (HAVE_LIBSCOTCH)
  find_package (LIBSCOTCH REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${Scotch_LIBRARIES})
  include_directories (${Scotch_INCLUDE_DIRS})
endif ()

target_link_libraries (neut ${EXTRA_LIBS})
