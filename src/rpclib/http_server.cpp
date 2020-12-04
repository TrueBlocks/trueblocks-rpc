/*-----------------------------------------------------------------------
 * This file was originally part of libjson-rpc-cpp which has been
 * almost completely re-written to remove anything not directly needed
 * by the Ethereum RPC. It retains the original license as described in
 * LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite circa 2020)
 *---------------------------------------------------------------------*/
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

#include <rpclib/http_server.h>
#include <rpclib/exception.h>

using namespace jsonrpc;
using namespace std;

//---------------------------------------------------------------------------------------
#define BUFFERSIZE 65536

//---------------------------------------------------------------------------------------
struct mhd_coninfo {
    struct MHD_PostProcessor* postprocessor;
    MHD_Connection* connection;
    stringstream request;
    HttpServer* server;
    int code;
};

//---------------------------------------------------------------------------------------
static void GetFileContent(const string& filename, string& target) {
    ifstream config(filename.c_str());
    if (config) {
        config.open(filename.c_str(), ios::in);
        target.assign((std::istreambuf_iterator<char>(config)), (std::istreambuf_iterator<char>()));
    } else {
        throw JsonRpcException(ERROR_SERVER_PROCEDURE_SPECIFICATION_NOT_FOUND, filename);
    }
}

//---------------------------------------------------------------------------------------
HttpServer::HttpServer(int p, const string& sslc, const string& sslk, int t)
    : handler(NULL),
      port(p),
      threads(t),
      running(false),
      path_sslcert(sslc),
      path_sslkey(sslk),
      daemon(NULL),
      bindlocalhost(false) {
}

//---------------------------------------------------------------------------------------
HttpServer::~HttpServer() {
}

//---------------------------------------------------------------------------------------
ServerProtocolHandler* HttpServer::GetHandler(const string& url) {
    if (handler != NULL)
        return handler;
    map<string, ServerProtocolHandler*>::iterator it = urlhandler.find(url);
    if (it != urlhandler.end())
        return it->second;
    return NULL;
}

//---------------------------------------------------------------------------------------
HttpServer& HttpServer::BindLocalhost() {
    bindlocalhost = true;
    return *this;
}

//---------------------------------------------------------------------------------------
bool HttpServer::StartListening() {
    if (!running) {
        const bool has_epoll = (MHD_is_feature_supported(MHD_FEATURE_EPOLL) == MHD_YES);
        const bool has_poll = (MHD_is_feature_supported(MHD_FEATURE_POLL) == MHD_YES);
        unsigned int mhd_flags = MHD_USE_DUAL_STACK;

        if (has_epoll)
// In MHD version 0.9.44 the flag is renamed to
// MHD_USE_EPOLL_INTERNALLY_LINUX_ONLY. In later versions both
// are deprecated.
#if defined(MHD_USE_EPOLL_INTERNALLY)
            mhd_flags = MHD_USE_EPOLL_INTERNALLY;
#else
            mhd_flags = MHD_USE_EPOLL_INTERNALLY_LINUX_ONLY;
#endif
        else if (has_poll)
            mhd_flags = MHD_USE_POLL_INTERNALLY;

        if (bindlocalhost) {
            memset(&loopback_addr, 0, sizeof(loopback_addr));
            loopback_addr.sin_family = AF_INET;
            loopback_addr.sin_port = htons(port);
            loopback_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

            daemon = MHD_start_daemon(mhd_flags, port, NULL, NULL, (MHD_AccessHandlerCallback)HttpServer::callback,
                                      this, MHD_OPTION_THREAD_POOL_SIZE, threads, MHD_OPTION_SOCK_ADDR,
                                      (struct sockaddr*)(&(loopback_addr)), MHD_OPTION_END);

        } else if (path_sslcert != "" && path_sslkey != "") {
            try {
                GetFileContent(path_sslcert, sslcert);
                GetFileContent(path_sslkey, sslkey);

                daemon = MHD_start_daemon(MHD_USE_SSL | mhd_flags, port, NULL, NULL,
                                          (MHD_AccessHandlerCallback)HttpServer::callback, this,
                                          MHD_OPTION_HTTPS_MEM_KEY, sslkey.c_str(), MHD_OPTION_HTTPS_MEM_CERT,
                                          sslcert.c_str(), MHD_OPTION_THREAD_POOL_SIZE, threads, MHD_OPTION_END);
            } catch (JsonRpcException& ex) {
                return false;
            }
        } else {
            daemon = MHD_start_daemon(mhd_flags, port, NULL, NULL, (MHD_AccessHandlerCallback)HttpServer::callback,
                                      this, MHD_OPTION_THREAD_POOL_SIZE, threads, MHD_OPTION_END);
        }
        if (daemon != NULL)
            running = true;
    }
    return running;
}

//---------------------------------------------------------------------------------------
bool HttpServer::StopListening() {
    if (running) {
        MHD_stop_daemon(daemon);
        running = false;
    }
    return true;
}

//---------------------------------------------------------------------------------------
bool HttpServer::SendResponse(const string& response, void* addInfo) {
    struct mhd_coninfo* client_connection = static_cast<struct mhd_coninfo*>(addInfo);
    struct MHD_Response* result =
        MHD_create_response_from_buffer(response.size(), (void*)response.c_str(), MHD_RESPMEM_MUST_COPY);

    MHD_add_response_header(result, "Content-Type", "application/json");
    MHD_add_response_header(result, "Access-Control-Allow-Origin", "*");

    int ret = MHD_queue_response(client_connection->connection, client_connection->code, result);
    MHD_destroy_response(result);
    return ret == MHD_YES;
}

//---------------------------------------------------------------------------------------
bool HttpServer::SendOptionsResponse(void* addInfo) {
    struct mhd_coninfo* client_connection = static_cast<struct mhd_coninfo*>(addInfo);
    struct MHD_Response* result = MHD_create_response_from_buffer(0, NULL, MHD_RESPMEM_MUST_COPY);

    MHD_add_response_header(result, "Allow", "POST, OPTIONS");
    MHD_add_response_header(result, "Access-Control-Allow-Origin", "*");
    MHD_add_response_header(result, "Access-Control-Allow-Headers", "origin, content-type, accept");
    MHD_add_response_header(result, "DAV", "1");

    int ret = MHD_queue_response(client_connection->connection, client_connection->code, result);
    MHD_destroy_response(result);
    return ret == MHD_YES;
}

//---------------------------------------------------------------------------------------
void HttpServer::SetUrlHandler(const string& url, ServerProtocolHandler* handler) {
    urlhandler[url] = handler;
    SetHandler(NULL);
}

//---------------------------------------------------------------------------------------
int HttpServer::callback(void* cls, MHD_Connection* connection, const char* url, const char* method,
                         const char* version, const char* upload_data, size_t* upload_data_size, void** con_cls) {
    (void)version;
    if (*con_cls == NULL) {
        struct mhd_coninfo* client_connection = new mhd_coninfo;
        client_connection->connection = connection;
        client_connection->server = static_cast<HttpServer*>(cls);
        *con_cls = client_connection;
        return MHD_YES;
    }
    struct mhd_coninfo* client_connection = static_cast<struct mhd_coninfo*>(*con_cls);

    if (string("POST") == method) {
        if (*upload_data_size != 0) {
            client_connection->request.write(upload_data, *upload_data_size);
            *upload_data_size = 0;
            return MHD_YES;
        } else {
            string response;
            ServerProtocolHandler* handler = client_connection->server->GetHandler(string(url));
            if (handler == NULL) {
                client_connection->code = MHD_HTTP_INTERNAL_SERVER_ERROR;
                client_connection->server->SendResponse("No client connection handler found", client_connection);
            } else {
                client_connection->code = MHD_HTTP_OK;
                handler->HandleRequest(client_connection->request.str(), response);
                client_connection->server->SendResponse(response, client_connection);
            }
        }
    } else if (string("OPTIONS") == method) {
        client_connection->code = MHD_HTTP_OK;
        client_connection->server->SendOptionsResponse(client_connection);
    } else {
        client_connection->code = MHD_HTTP_METHOD_NOT_ALLOWED;
        client_connection->server->SendResponse("Not allowed HTTP Method", client_connection);
    }

    if (client_connection != nullptr) {
        delete client_connection;
    }
    *con_cls = NULL;

    return MHD_YES;
}

//---------------------------------------------------------------------------------------
void HttpServer::ProcessRequest(const string& request, string& response) {
    if (handler != NULL) {
        handler->HandleRequest(request, response);
    }
}

//---------------------------------------------------------------------------------------
void HttpServer::SetHandler(ServerProtocolHandler* h) {
    handler = h;
}

//---------------------------------------------------------------------------------------
ServerProtocolHandler* HttpServer::GetHandler() {
    return handler;
}
