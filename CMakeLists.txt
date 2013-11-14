cmake_minimum_required(VERSION 2.8)

project(LIBLDSC)

# Get the date
MACRO(TODAY RESULT YEAR MONTH DAY)
    IF(WIN32)
        EXECUTE_PROCESS(COMMAND "cmd" " /C date /T" OUTPUT_VARIABLE ${RESULT})
        #string(REGEX REPLACE "(..)/(..)/..(..).*" "\\1/\\2/\\3" ${RESULT} ${${RESULT}})
        string(REGEX REPLACE "(..)/(..)/(....).*" "\\3" ${YEAR} ${${RESULT}})
        string(REGEX REPLACE "(..)/(..)/(....).*" "\\2" ${MONTH} ${${RESULT}})
        string(REGEX REPLACE "(..)/(..)/(....).*" "\\1" ${DAY} ${${RESULT}})
    ELSEIF(UNIX)
        EXECUTE_PROCESS(COMMAND "date" "+%d/%m/%Y" OUTPUT_VARIABLE ${RESULT})
        #string(REGEX REPLACE "(..)/(..)/..(..).*" "\\1/\\2/\\3" ${RESULT} ${${RESULT}})
        string(REGEX REPLACE "(..)/(..)/(....).*" "\\3" ${YEAR} ${${RESULT}})
        string(REGEX REPLACE "(..)/(..)/(....).*" "\\2" ${MONTH} ${${RESULT}})
        string(REGEX REPLACE "(..)/(..)/(....).*" "\\1" ${DAY} ${${RESULT}})
    ELSE(WIN32)
        MESSAGE(SEND_ERROR "date not implemented")
        SET(${RESULT} 000000)
        SET(${YEAR} 9999)
        SET(${MONTH} 99)
        SET(${DAY} 99)
    ENDIF(WIN32)
ENDMACRO(TODAY)

TODAY(RESULT YEAR MONTH DAY)
message(STATUS "Compilation date = ${RESULT}")

# The version number.
set(${PROJECT_NAME}_VERSION_MAJOR ${YEAR})
set(${PROJECT_NAME}_VERSION_MINOR ${MONTH})
set(${PROJECT_NAME}_PATCH_LEVEL ${DAY})

# Detecting C++11 support
#if(CMAKE_COMPILER_IS_GNUCXX)
#   execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
#   if (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
#        message(STATUS "C++11 activated.")
#        add_definitions("-std=gnu++11")
#   elseif(GCC_VERSION VERSION_GREATER 4.3 OR GCC_VERSION VERSION_EQUAL 4.3)
#        message(WARNING "C++0x activated. If you get any errors update to a compiler which fully supports C++11")
#        add_definitions("-std=gnu++0x")
#   else ()
#        message(FATAL_ERROR "C++11 needed. Therefore a gcc compiler with a version higher than 4.3 is needed.")
#   endif()
#else(CMAKE_COMPILER_IS_GNUCXX)
#   add_definitions("-std=c++0x")
#endif(CMAKE_COMPILER_IS_GNUCXX)

FIND_PACKAGE( OpenMP REQUIRED)

if(UNIX) # True for UNIX and UNIX like operating systems.
  set(CMAKE_CXX_FLAGS_RELEASE "-O2 -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__ -D_GNU_SOURCE -fopenmp -lgomp")
endif()
if(WIN32) # True on windows systems, including win64
  set(CMAKE_CXX_FLAGS_RELEASE "-O2 -fopenmp -lgomp")
  set(CMAKE_CXX_FLAGS "${OpenMP_CXX_FLAGS}")
  #set_target_properties(ldsc PROPERTIES LINK_FLAGS_RELEASE "-O2 -fopenmp -lgomp")
endif()

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_BUILD_TYPE Release)
#set (CMAKE_BUILD_TYPE Debug)
#set(PATH_LIB_LDSC "/home/puerari/desenv/libldsc/src/")

#file(MAKE_DIRECTORY lib)
#set(LIBRARY_OUTPUT_DIRECTORY lib)

#include_directories (${PATH_LIB_LDSC})
include_directories(${LIBLDSC_SOURCE_DIR}/src/)
link_directories(${LIBLDSC_SOURCE_DIR}/)

#message(FATAL_ERROR \${CMAKE_BUILD_TYPE}=${CMAKE_BUILD_TYPE})
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(demo)

if(WIN32) # True on windows systems, including win64
  set_target_properties(ldsc PROPERTIES LINK_FLAGS_RELEASE "-O2 -fopenmp -lgomp")
  set_target_properties(ldsc PROPERTIES LINK_FLAGS "-O2 -fopenmp -lgomp")
endif()

# Create libldsc executable.
#add_executable(libldsc src/libldsc.cpp)
