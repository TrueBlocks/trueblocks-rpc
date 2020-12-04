file(GLOB_RECURSE rpctgen_source *.cpp)

include_directories(..)
add_executable(rpc_gen ${rpctgen_source})
target_link_libraries(rpc_gen jsonrpclibs ${ARGTABLE_LIBRARIES})
