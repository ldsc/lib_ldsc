cmake_minimum_required (VERSION 2.8)

project(LIBLDSC)

# The version number.
set(${PROJECT_NAME}_VERSION_MAJOR 2011) 
set(${PROJECT_NAME}_VERSION_MINOR 03)
set(${PROJECT_NAME}_PATCH_LEVEL 30)

set(PATH_LIB_LDSC "/home/puerari/desenv/libldsc/src/")
set(CMAKE_CXX_FLAGS_RELEASE "-I${PATH_LIB_LDSC}")
#message(FATAL_ERROR ${PATH_LIB_LDSC})

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_BUILD_TYPE Release)
#message(FATAL_ERROR \${CMAKE_BUILD_TYPE}=${CMAKE_BUILD_TYPE})

add_subdirectory(src)