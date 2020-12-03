file(GLOB_RECURSE rpctgen_source *.cpp)

include_directories(..)
add_executable(rpctgen ${rpctgen_source})
target_link_libraries(rpctgen jsonrpccommon ${ARGTABLE_LIBRARIES})
