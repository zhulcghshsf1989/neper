cmake_minimum_required(VERSION 2.6)
project(neper C)

set (NEPER_VERSION_MAJOR 2)
set (NEPER_VERSION_MINOR 0)
set (NEPER_VERSION_PATCH 0)
set (NEPER_VERSION \"2.0.0-devel2-57\ \")
set (NEPER_EMAIL \"neper-users@lists.sourceforge.net\")

# should we use the optional libraries?
option (HAVE_GSL         "Use the GNU Scientific Library (GSL)" ON )
option (HAVE_LIBMATHEVAL "Use libmatheval"                      ON )
option (HAVE_LIBSCOTCH   "Use libscotch"                        OFF)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/contrib/ut")
include_directories("${PROJECT_SOURCE_DIR}/contrib/orilib")
include_directories("${PROJECT_SOURCE_DIR}/Structs")
include_directories("${PROJECT_SOURCE_DIR}/neut")
include_directories("${PROJECT_SOURCE_DIR}/neper_t")
include_directories("${PROJECT_SOURCE_DIR}/neper_m")
include_directories("${PROJECT_SOURCE_DIR}/neper_o")
# include_directories("${PROJECT_SOURCE_DIR}/neper_mm")
include_directories("${PROJECT_SOURCE_DIR}/neper_vs")

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_BINARY_DIR}/contrib/ut")
include_directories("${PROJECT_BINARY_DIR}/contrib/orilib")
include_directories("${PROJECT_SOURCE_DIR}/Structs")
include_directories("${PROJECT_BINARY_DIR}/neut")
include_directories("${PROJECT_BINARY_DIR}/neper_t")
include_directories("${PROJECT_BINARY_DIR}/neper_m")
include_directories("${PROJECT_BINARY_DIR}/neper_o")
# include_directories("${PROJECT_BINARY_DIR}/neper_mm")
include_directories("${PROJECT_BINARY_DIR}/neper_vs")

# configure a header file to pass some of the CMake settings to the
# source code
configure_file (
  "${PROJECT_SOURCE_DIR}/neper_config.h.in"
  "${PROJECT_BINARY_DIR}/neper_config.h"
)

add_definitions(-Wall -W -g -O3)
set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})

add_subdirectory(contrib/ut)
add_subdirectory(contrib/orilib)
add_subdirectory(neut)
add_subdirectory(neper_t)
add_subdirectory(neper_m)
add_subdirectory(neper_o)
# add_subdirectory(neper_mm)
add_subdirectory(neper_vs)

add_executable (neper mp.c)

set (EXTRA_LIBS ${EXTRA_LIBS} ut)
set (EXTRA_LIBS ${EXTRA_LIBS} orilib)
set (EXTRA_LIBS ${EXTRA_LIBS} neut)
set (EXTRA_LIBS ${EXTRA_LIBS} neper_t)
set (EXTRA_LIBS ${EXTRA_LIBS} neper_m)
set (EXTRA_LIBS ${EXTRA_LIBS} neper_o)
# set (EXTRA_LIBS ${EXTRA_LIBS} neper_mm)
set (EXTRA_LIBS ${EXTRA_LIBS} neper_vs)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

if (HAVE_LIBSCOTCH)
  find_package (LIBSCOTCH REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${Scotch_LIBRARIES})
  include_directories (${Scotch_INCLUDE_DIRS})
  if (LIBSCOTCH)
    message ("Found libscotch")
  endif (LIBSCOTCH)
endif ()

target_link_libraries (neper ${EXTRA_LIBS})

install (TARGETS neper DESTINATION bin
         PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ
	 GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
