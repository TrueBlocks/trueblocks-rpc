cmake_minimum_required(VERSION 3.2)

file(GLOB_RECURSE RCPGEN_SOURCE *.cpp)

debug_value("RCPGEN_SOURCE:        " ${RCPGEN_SOURCE})
include_directories(..)
add_executable(rpc_gen ${RCPGEN_SOURCE})
target_link_libraries(rpc_gen rpc_lib ${ARGTABLE_LIBRARIES})
