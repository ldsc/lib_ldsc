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

#set(PATH_LIB_LDSC "/home/puerari/desenv/libldsc/src/")
if(UNIX) # True for UNIX and UNIX like operating systems.
  set (CMAKE_CXX_FLAGS_RELEASE "-O2 -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__ -D_GNU_SOURCE")
endif()
if(WIN32) # True on windows systems, including win64
  set (CMAKE_CXX_FLAGS_RELEASE "-O2")
endif()
#include_directories (${PATH_LIB_LDSC})
include_directories(${LIBLDSC_SOURCE_DIR}/src/)
link_directories(${LIBLDSC_SOURCE_DIR}/src/)
#message(FATAL_ERROR ${PATH_LIB_LDSC})

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_BUILD_TYPE Release)
#set (CMAKE_BUILD_TYPE Debug)
#set(LIBRARY_OUTPUT_DIRECTORY lib)
#message(FATAL_ERROR \${CMAKE_BUILD_TYPE}=${CMAKE_BUILD_TYPE})
add_subdirectory(src)
#add_subdirectory(demo)
add_subdirectory(test)

add_executable(libldsc src/libldsc.cpp)
