cmake_minimum_required(VERSION 3.2)

file(GLOB jsonrpc_header_libs *.h)
file(GLOB jsonrpc_source_libs *.c*)

include_directories(..)
include_directories(${MHD_INCLUDE_DIRS})

add_library(rpc_lib STATIC ${jsonrpc_source_libs} ${jsonrpc_header_libs} ${jsonrpc_header_libs} "http_client.cpp" "http_server.cpp")
target_link_libraries(rpc_lib jsoncpp_lib_static ${CMAKE_THREAD_LIBS_INIT} ${MHD_LIBRARIES} ${CURL_LIBRARIES})
set_target_properties(rpc_lib PROPERTIES OUTPUT_NAME rpclib)

set(ALL_LIBS)
list(APPEND ALL_LIBS rpc_lib)
set_target_properties(${ALL_LIBS} PROPERTIES VERSION "${VERSION_STRING}" SOVERSION "${SO_VERSION}")
