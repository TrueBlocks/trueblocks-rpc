file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/gen)

include_directories(..)
include_directories(${CMAKE_BINARY_DIR})
include_directories(${MHD_INCLUDE_DIRS})

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/gen/rpct_server.h
    COMMAND rpctgen ARGS ${CMAKE_CURRENT_SOURCE_DIR}/../spec.json --cpp-server=AbstractStubServer --cpp-server-file=${CMAKE_BINARY_DIR}/gen/rpct_server.h
    MAIN_DEPENDENCY ../spec.json
    DEPENDS rpctgen
    COMMENT "Generating C++ Server Stubfiles"
    VERBATIM
)

add_custom_target(common_server_stubs DEPENDS ${CMAKE_BINARY_DIR}/gen/rpct_server.h)

add_executable(rpctd rpct_server.cpp ${CMAKE_BINARY_DIR}/gen/rpct_server.h)
target_link_libraries(rpctd jsonrpcserver)
