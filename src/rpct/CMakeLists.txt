# setup libs headers and sources
file(GLOB jsonrpc_header *.h)
file(GLOB jsonrpc_header_libs libs/*.h)
file(GLOB jsonrpc_source_libs libs/*.c*)

# setup server headers and sources
file(GLOB jsonrpc_install_header_server
	server/abstractserver.h
	server/abstractserverconnector.h
        server/abstractthreadedserver.h
	server/iprocedureinvokationhandler.h
	server/iclientconnectionhandler.h
)
file(GLOB jsonrpc_header_server server/*.h)
file(GLOB jsonrpc_source_server server/*.c*)

# setup client headers and sources
file(GLOB jsonrpc_install_header_client
    client/batchcall.h
    client/batchresponse.h
    client/client.h
    client/iclientconnector.h
)
file(GLOB jsonrpc_header_client client/*.h)
file(GLOB jsonrpc_source_client client/*.c*)

# setup connector variables defaults
set(client_connector_source "")
set(client_connector_header "")
set(client_connector_libs   "")
set(server_connector_source "")
set(server_connector_header "")
set(server_connector_libs   "")
set(SERVER_LIBS "")
set(CLIENT_LIBS "")

# setup sources for http connectors
list(APPEND client_connector_header "client/httpclient.h")
list(APPEND client_connector_source "client/httpclient.cpp")
list(APPEND client_connector_libs ${CURL_LIBRARIES})
set(CLIENT_LIBS "${CLIENT_LIBS} -lcurl")

list(APPEND server_connector_header "server/httpserver.h")
list(APPEND server_connector_source "server/httpserver.cpp")
list(APPEND server_connector_libs ${CMAKE_THREAD_LIBS_INIT} ${MHD_LIBRARIES})
set(SERVER_LIBS "${SERVER_LIBS} -lmicrohttpd")

# configure a header file to pass some of the CMake settings to the source code
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/gen/rpct/libs")
configure_file("${PROJECT_SOURCE_DIR}/rpct/version.h.in" "${CMAKE_BINARY_DIR}/gen/rpct/version.h")
install(FILES "${CMAKE_BINARY_DIR}/gen/rpct/version.h" DESTINATION include/rpct)

# include required directories
include_directories(..)
include_directories(${MHD_INCLUDE_DIRS})

add_library(libs STATIC ${jsonrpc_source_libs} ${jsonrpc_header})
target_link_libraries(libs jsoncpp_lib_static)
set_target_properties(libs PROPERTIES OUTPUT_NAME rpct-libs)
add_library(jsonrpclibs ALIAS libs)

add_library(client STATIC ${jsonrpc_source_client} ${jsonrpc_header} ${jsonrpc_header_client} ${client_connector_source})
target_link_libraries(client libs ${client_connector_libs})
set_target_properties(client PROPERTIES OUTPUT_NAME rpct-client)
add_library(jsonrpcclient ALIAS client)

add_library(server STATIC ${jsonrpc_source_server} ${jsonrpc_header} ${jsonrpc_header_server} ${server_connector_source})
target_link_libraries(server libs ${server_connector_libs})
set_target_properties(server PROPERTIES OUTPUT_NAME rpct-server)
add_library(jsonrpcserver ALIAS server)

set(ALL_LIBS)
list(APPEND ALL_LIBS libs client server)
set_target_properties(${ALL_LIBS} PROPERTIES VERSION "${VERSION_STRING}" SOVERSION "${SO_VERSION}")

# install libraries
install(FILES ${jsonrpc_header}                 DESTINATION include/rpct)
install(FILES ${jsonrpc_header_libs}            DESTINATION include/rpct/libs)
install(FILES ${jsonrpc_helper_header_libs}     DESTINATION include/rpct/libs/helper)
install(FILES ${jsonrpc_install_header_client}  DESTINATION include/rpct/client)
install(FILES ${jsonrpc_install_header_server}  DESTINATION include/rpct/server)
install(FILES ${client_connector_header}        DESTINATION include/rpct/client)
install(FILES ${server_connector_header}        DESTINATION include/rpct/server)

if (WIN32 AND NOT HUNTER_ENABLED)
	# On Windows copy manually installed dependencies unless Hunter is used.
	file(GLOB win32_libs ${CMAKE_SOURCE_DIR}/win32-deps/lib/*.dll)
	file(COPY ${win32_libs} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	install(FILES ${win32_libs} DESTINATION bin)
	install(DIRECTORY ${CMAKE_SOURCE_DIR}/win32-deps/include DESTINATION .)
endif()

install(TARGETS ${ALL_LIBS} EXPORT libjson-rpc-cppTargets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION bin
)

#set pkg-config
#get_filename_component(FULL_PATH_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}" ABSOLUTE)
#set(FULL_PATH_INCLUDEDIR "${FULL_PATH_INSTALL_PREFIX}/include")
#set(FULL_PATH_LIBDIR "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

#configure_file(${PROJECT_SOURCE_DIR}/cmake/libjsonrpccpp-client.pc.cmake ${CMAKE_BINARY_DIR}/libjsonrpccpp-client.pc)
#configure_file(${PROJECT_SOURCE_DIR}/cmake/libjsonrpccpp-server.pc.cmake ${CMAKE_BINARY_DIR}/libjsonrpccpp-server.pc)
#configure_file(${PROJECT_SOURCE_DIR}/cmake/libjsonrpccpp-libs.pc.cmake ${CMAKE_BINARY_DIR}/libjsonrpccpp-libs.pc)
#
#INSTALL(FILES "${CMAKE_BINARY_DIR}/libjsonrpccpp-server.pc" "${CMAKE_BINARY_DIR}/libjsonrpccpp-client.pc" "${CMAKE_BINARY_DIR}/libjsonrpccpp-libs.pc" DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
