file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/gen)

include_directories(..)
include_directories(${CMAKE_BINARY_DIR})
include_directories(${MHD_INCLUDE_DIRS})

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/gen/rpc_server.h
    COMMAND rpc_generator ARGS ${CMAKE_CURRENT_SOURCE_DIR}/../spec.json --cpp-server=AbstractStubServer --cpp-server-file=${CMAKE_BINARY_DIR}/gen/rpc_server.h
    MAIN_DEPENDENCY ../spec.json
    DEPENDS rpc_generator
    COMMENT "Generating C++ Server Stubfiles"
    VERBATIM
)

add_custom_target(common_server_stubs DEPENDS ${CMAKE_BINARY_DIR}/gen/rpc_server.h)

add_executable(rpc_server rpc_server.cpp ${CMAKE_BINARY_DIR}/gen/rpc_server.h)
target_link_libraries(rpc_server jsonrpcserver)
