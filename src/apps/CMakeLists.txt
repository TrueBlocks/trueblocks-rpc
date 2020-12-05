cmake_minimum_required(VERSION 3.2)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/gen)

include_directories(..)
include_directories(${CMAKE_BINARY_DIR})

set(SERVER_HEADER ${CMAKE_BINARY_DIR}/gen/rpc_server.h)
debug_value("SERVER_HEADER:        " ${SERVER_HEADER})
add_custom_command(
    OUTPUT ${SERVER_HEADER}
    COMMAND rpc_gen ARGS ${CMAKE_CURRENT_SOURCE_DIR}/../spec.json --cpp-server=AbstractServer --cpp-server-file=${SERVER_HEADER}
    MAIN_DEPENDENCY ../spec.json
    DEPENDS rpc_gen
    COMMENT "Generating C++ Server Stubfiles"
    VERBATIM
)
add_executable(rpc_server rpc_server.cpp ${SERVER_HEADER})
target_link_libraries(rpc_server rpc_lib)

set(CLIENT_HEADER ${CMAKE_BINARY_DIR}/gen/rpc_client.h)
debug_value("CLIENT_HEADER:        " ${CLIENT_HEADER})
add_custom_command(
    OUTPUT ${CLIENT_HEADER}
    COMMAND rpc_gen ARGS ${CMAKE_CURRENT_SOURCE_DIR}/../spec.json --cpp-client=RpctClient --cpp-client-file=${CLIENT_HEADER}
    MAIN_DEPENDENCY ../spec.json
    DEPENDS rpc_gen
    COMMENT "Generating C++ Client Stubfile"
    VERBATIM
)
add_executable(rpc_client rpc_client.cpp ${CLIENT_HEADER})
target_link_libraries(rpc_client rpc_lib)
