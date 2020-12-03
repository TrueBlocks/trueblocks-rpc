file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/gen)

include_directories(..)
include_directories(${CMAKE_BINARY_DIR})
include_directories(${MHD_INCLUDE_DIRS})

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/gen/rpct_client.h
    COMMAND rpctgen ARGS ${CMAKE_CURRENT_SOURCE_DIR}/../spec.json --cpp-client=RpctClient --cpp-client-file=${CMAKE_BINARY_DIR}/gen/rpct_client.h
    MAIN_DEPENDENCY ../spec.json
    DEPENDS rpctgen
    COMMENT "Generating C++ Client Stubfile"
    VERBATIM
)

add_custom_target(common_client_stubs DEPENDS ${CMAKE_BINARY_DIR}/gen/rpct_client.h)

add_executable(rpctc rpct_client.cpp ${CMAKE_BINARY_DIR}/gen/rpct_client.h)
target_link_libraries(rpctc jsonrpcclient)
