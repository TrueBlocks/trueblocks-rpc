file(GLOB_RECURSE rpctgen_source *.cpp)

include_directories(..)
add_executable(rpc_generator ${rpctgen_source})
target_link_libraries(rpc_generator jsonrpccommon ${ARGTABLE_LIBRARIES})
