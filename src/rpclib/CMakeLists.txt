file(GLOB jsonrpc_header_libs *.h)
file(GLOB jsonrpc_source_libs *.c*)

include_directories(..)
include_directories(${MHD_INCLUDE_DIRS})

add_library(libs STATIC ${jsonrpc_source_libs} ${jsonrpc_header_libs} ${jsonrpc_header_libs} "httpclient.cpp" "httpserver.cpp")
target_link_libraries(libs jsoncpp_lib_static ${CMAKE_THREAD_LIBS_INIT} ${MHD_LIBRARIES} ${CURL_LIBRARIES})
set_target_properties(libs PROPERTIES OUTPUT_NAME rpclib-libs)
add_library(jsonrpclibs ALIAS libs)

set(ALL_LIBS)
list(APPEND ALL_LIBS libs)
set_target_properties(${ALL_LIBS} PROPERTIES VERSION "${VERSION_STRING}" SOVERSION "${SO_VERSION}")
