cmake_minimum_required(VERSION 2.6)
project(neper_fm C)

option (HAVE_GSL         "Use the GNU Scientific Library (GSL)" ON )
option (HAVE_LIBMATHEVAL "Use libmatheval"                      ON )
option (HAVE_LIBSCOTCH   "Use libscotch"                        OFF)

include_directories("${PROJECT_BINARY_DIR}")

add_definitions(-Wall -W -g -O3)

add_library (neper_fm
./RegularizeGeo/MergeGeo/MergeGeo2.c
./RegularizeGeo/MergeGeo/MergeGeo1.c
./RegularizeGeo/MergeGeo/DeleteEdge/DeleteEdge1.c
./RegularizeGeo/MergeGeo/DeleteEdge/DeleteEdge2.c
./RegularizeGeo/MergeGeo/DeleteEdge/FaceFF/FaceFFPlanes2.c
./RegularizeGeo/MergeGeo/DeleteEdge/FaceFF/FaceFFPlanes1.c
./RegularizeGeo/MergeGeo/DeleteEdge/FaceFF/FaceFF1.c
./RegularizeGeo/MergeGeo/DeleteEdge/DeleteEdge3.c
./RegularizeGeo/MergeGeo/DeleteEdge/DeleteEdge5.c
./RegularizeGeo/MergeGeo/DeleteEdge/DeleteEdge4.c
./RegularizeGeo/RegularizeGeo1.c
./MStat/MStat1.c
./MStat/MStat2.c
./MStat/MStat3.c
./ReadMesh/ReadMesh1.c
./Meshing0D/Meshing0D2.c
./Meshing0D/Meshing0D1.c
./InputData_fm/InputData_fm1.c
./InputData_fm/InputData_fm2.c
./InputData_fm/InputData_fm3.c
./Premeshing/Premeshing2.c
./Premeshing/Premeshing1.c
./nefm/nefm_mesh_stellar/nefm_mesh_stellar1.c
./nefm/nefm_mesh_stellar/nefm_mesh_stellar2.c
./nefm/nefm_geo/nefm_geo1.c
./nefm/nefm_mesh_gmsh/nefm_mesh_gmsh3.c
./nefm/nefm_mesh_gmsh/nefm_mesh_gmsh2.c
./nefm/nefm_mesh_gmsh/nefm_mesh_gmsh1.c
./GeoStuff/GeoStuff2.c
./GeoStuff/GeoStuff1.c
./nefm_init/nefm_init1.c
./nefm_init/nefm_init2.c
./Meshing3D/Meshing3D1.c
./Meshing3D/Meshing3D2.c
./Meshing1D/Meshing1D2.c
./Meshing1D/Meshing1D1.c
./Meshing1D/Meshing1D3.c
./Remapping/Remapping2.c
./Remapping/Remapping1.c
./Partition/Partition5.c
./Partition/Partition1.c
./Partition/Partition4.c
./Partition/Partition3.c
./Partition/Partition2.c
./Meshing2D/Meshing2D1.c
./Remeshing/Remeshing2.c
./Remeshing/Remeshing1.c
./ReconMesh/ReconMesh1.c
./ReconMesh/ReconMesh2.c
./WriteMesh/WriteRemesh/WriteRemesh1.c
./WriteMesh/WriteMeshAbq/WriteMeshAbq1.c
./WriteMesh/WriteMeshAbq/WriteMeshAbq2.c
./WriteMesh/WriteMesh1.c
./WriteMesh/WritePart/WritePart1.c
./WriteMesh/WriteMeshFev/WriteMeshFev1.c
./WriteMesh/WriteMeshFev/WriteMeshFev2.c
./WriteMesh/WriteMeshFev/WriteMeshFev3.c
./WriteMesh/WriteMeshGeof/WriteMeshGeof1.c
./WriteMesh/WriteMeshGeof/WriteMeshGeof2.c
./WriteMesh/WriteMeshGeof/WriteMeshGeof3.c
./WriteMesh/WriteMeshScotch/WriteMeshScotch1.c
./SearchNSets/SearchNSets1.c
./SearchNSets/SearchNSets2.c
./neper_fm.c
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

target_link_libraries (neper_fm ${EXTRA_LIBS})
