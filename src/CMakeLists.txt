cmake_minimum_required(VERSION 3.0)

set(DEBUG_BUILD YES)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../cmake")

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

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${CMAKE_SOURCE_DIR}/win32-deps")

find_package(Jsoncpp REQUIRED)
debug_value("Jsoncpp header:  " ${JSONCPP_INCLUDE_DIR})
debug_value("Jsoncpp lib:     " ${JSONCPP_LIBRARY})

find_package(Argtable REQUIRED)
debug_value("Argtable header: " ${ARGTABLE_INCLUDE_DIRS})
debug_value("Argtable lib:    " ${ARGTABLE_LIBRARIES})

find_package(CURL REQUIRED)
debug_value("CURL header:     " ${CURL_INCLUDE_DIRS})
debug_value("CURL lib:        " ${CURL_LIBRARIES})

find_package(MHD REQUIRED)
debug_value("MHD header:      " ${MHD_INCLUDE_DIRS})
debug_value("MHD lib:         " ${MHD_LIBRARIES})

find_package(Threads REQUIRED)
# debug_value("Threads header:  " ${THREADS_INCLUDE_DIRS})
# debug_value("Threads lib:     " ${THREADS_LIBRARIES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

include_directories(${JSONCPP_INCLUDE_DIR})
include_directories(${JSONCPP_INCLUDE_DIR}/jsoncpp)
include(GNUInstallDirs)
include_directories("${CMAKE_BINARY_DIR}/gen/")

add_subdirectory(rpct)
add_subdirectory(gen)
add_subdirectory(apps)
