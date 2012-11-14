cmake_minimum_required(VERSION 2.6)
project(Utils)

# should we use the optional libraries?
option (HAVE_GSL      "Use the GNU Scientific Library (GSL)" ON)

include_directories("${PROJECT_BINARY_DIR}")

add_definitions(-Wall -W -O3)

add_library (Utils
./TessDesUtils/TessDesUtils.c
./VectorUtils/VectorUtils1.c
./TensorUtils/TensorUtils.c
./FileUtils/FileUtils.c
./MshUtils/FaceNodes/FaceNodes1.c
./MshUtils/Flatness/Flatness2.c
./MshUtils/Flatness/Flatness3.c
./MshUtils/Flatness/Flatness4.c
./MshUtils/Flatness/Flatness1.c
./CharUtils/CharUtils.c
./FEUtils/FEUtils1.c
./MMeshUtils/MMeshUtils2.c
./MMeshUtils/MMeshUtils1.c
./MMeshUtils/MMeshUtils3.c
./SpaceUtils/SpaceUtils1.c
./SpaceUtils/SpaceUtils2.c
./MeshUtils/MeshUtils2.c
./MeshUtils/MeshUtils1.c
./MatrixUtils/VectorUtils.c
./MatrixUtils/MatrixUtils.c
./GeoUtils/Various/Various1.c
./GeoUtils/PrintGeoAsX/PrintGeoAsX2.c
./GeoUtils/PrintGeoAsX/PrintGeoAsX1.c
./TessUtils/T2P/T2P2.c
./TessUtils/T2P/T2P3.c
./TessUtils/T2P/T2P1.c
./TessUtils/InPoly/InPoly1.c
./TessUtils/ZerosStruct.c
./TessUtils/FreeStruct.c
./TessUtils/P2Pm/P2Pm1.c
./TessUtils/P2Pm/P2Pm2.c
./TessUtils/P2Pm/P2Pm3.c
./TessUtils/P2Pg/P2Pg1.c
./TessUtils/P2Pg/P2Pg2.c
./NumUtils/NumUtils.c
./ArrayUtils/FreeUtils.c
./ArrayUtils/OperUtils.c
./ArrayUtils/AllocUtils.c
./ArrayUtils/ZerosUtils.c
./GslUtils/GslUtils.c
./DisplayUtils/DisplayUtils.c
./OptionUtils/OptionUtils.c
)


set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake
			${CMAKE_MODULE_PATH})

set (EXTRA_LIBS ${EXTRA_LIBS} ut)

if (HAVE_GSL)
  find_package(GSL REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${GSL_LIBRARIES})
endif (HAVE_GSL)

if (HAVE_LIBSCOTCH)
  find_package (LIBSCOTCH REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${Scotch_LIBRARIES})
  include_directories (${Scotch_INCLUDE_DIRS})
endif ()

target_link_libraries (Utils ${EXTRA_LIBS})
