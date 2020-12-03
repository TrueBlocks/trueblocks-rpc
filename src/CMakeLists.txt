cmake_minimum_required(VERSION 3.0)

set(DEBUG_BUILD YES)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

option(HUNTER_ENABLED "Enable Hunter package manager" NO)
include(HunterGate)
HunterGate(
	URL "https://github.com/ruslo/hunter/archive/v0.18.47.tar.gz"
	SHA1 "f0c476a3a0f9edd6182936d8c4460811f112189c"
)

project(libjson-rpc-cpp)

cmake_policy(SET CMP0007 NEW)
cmake_policy(SET CMP0012 NEW)
cmake_policy(SET CMP0042 NEW)

set(MAJOR_VERSION 0)
set(MINOR_VERSION 1)
set(PATCH_VERSION 0)
set(SO_VERSION    1)
set(VERSION_STRING ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

function (debug_value name value)
    if(DEBUG_BUILD)
        message(STATUS "debug --> ${name} ${value}")
    endif()
endfunction()

debug_value("CMAKE_SOURCE_DIR:  " ${CMAKE_SOURCE_DIR})
debug_value("CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})
debug_value("VERSION_STRING:    " ${VERSION_STRING})

set(LIB_SUFFIX "" CACHE STRING "Suffix for library directory (32/64)")

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wformat -Wno-format-extra-args -Wformat-security -Wformat-nonliteral -Wformat=2 -Wextra -Wnon-virtual-dtor -fPIC -O0")

elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -std=c++17 -Wall -Wextra -Wnon-virtual-dtor -fPIC -O0")

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # Suppress warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -wd4290")

endif()

# all dependencies that are not directly included in the libjson-rpc-cpp distribution are defined here!
# default search directory for dependencies is ${CMAKE_SOURCE_DIR}/win32-deps (for backwards compatibility)
# if your dependencies directory is different, please run cmake with CMAKE_PREFIX_PATH option eg:
#
# cmake -DCMAKE_PREFIX_PATH=path_to_your_dependencies .

# set default dependencies search path
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${CMAKE_SOURCE_DIR}/win32-deps")

find_package(Jsoncpp REQUIRED)
message(STATUS "Jsoncpp header: ${JSONCPP_INCLUDE_DIR}")
message(STATUS "Jsoncpp lib   : ${JSONCPP_LIBRARY}")
include_directories(${JSONCPP_INCLUDE_DIR})
include_directories(${JSONCPP_INCLUDE_DIR}/jsoncpp)

find_package(Argtable REQUIRED)
message(STATUS "Argtable header: ${ARGTABLE_INCLUDE_DIRS}")
message(STATUS "Argtable lib   : ${ARGTABLE_LIBRARIES}")

find_package(CURL REQUIRED)
find_package(Threads)
message(STATUS "CURL header: ${CURL_INCLUDE_DIRS}")
message(STATUS "CURL lib   : ${CURL_LIBRARIES}")

find_package(MHD REQUIRED)
message(STATUS "MHD header: ${MHD_INCLUDE_DIRS}")
message(STATUS "MHD lib   : ${MHD_LIBRARIES}")

find_package(Threads REQUIRED)

include(GNUInstallDirs)

# setup output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# setup global include_directories (for backwards compatibility)
include_directories("${CMAKE_BINARY_DIR}/gen/")
include_directories("${CMAKE_BINARY_DIR}/gen/rpct/libs")

add_subdirectory(rpct)
add_subdirectory(generator)
add_subdirectory(server)
add_subdirectory(client)
