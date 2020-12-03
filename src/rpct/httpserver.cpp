/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpserver.cpp
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include <rpct/httpserver.h>
#include <rpct/exception.h>

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace jsonrpc;
using namespace std;

#define BUFFERSIZE 65536

struct mhd_coninfo {
    struct MHD_PostProcessor* postprocessor;
    MHD_Connection* connection;
    stringstream request;
    HttpServer* server;
    int code;
};

static void GetFileContent(const string& filename, string& target) {
    ifstream config(filename.c_str());
    if (config) {
        config.open(filename.c_str(), ios::in);
        target.assign((std::istreambuf_iterator<char>(config)), (std::istreambuf_iterator<char>()));
    } else {
        throw JsonRpcException(Errors::ERROR_SERVER_PROCEDURE_SPECIFICATION_NOT_FOUND, filename);
    }
}

HttpServer::HttpServer(int port, const string& sslcert, const string& sslkey, int threads)
    : handler(NULL),
      port(port),
      threads(threads),
      running(false),
      path_sslcert(sslcert),
      path_sslkey(sslkey),
      daemon(NULL),
      bindlocalhost(false) {
}

HttpServer::~HttpServer() {
}

ProtocolHandler* HttpServer::GetHandler(const string& url) {
    if (handler != NULL)
        return handler;
    map<string, ProtocolHandler*>::iterator it = this->urlhandler.find(url);
    if (it != this->urlhandler.end())
        return it->second;
    return NULL;
}

HttpServer& HttpServer::BindLocalhost() {
    this->bindlocalhost = true;
    return *this;
}

bool HttpServer::StartListening() {
    if (!this->running) {
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

        if (this->bindlocalhost) {
            memset(&this->loopback_addr, 0, sizeof(this->loopback_addr));
            loopback_addr.sin_family = AF_INET;
            loopback_addr.sin_port = htons(this->port);
            loopback_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

            this->daemon =
                MHD_start_daemon(mhd_flags, this->port, NULL, NULL, (MHD_AccessHandlerCallback)HttpServer::callback,
                                 this, MHD_OPTION_THREAD_POOL_SIZE, this->threads, MHD_OPTION_SOCK_ADDR,
                                 (struct sockaddr*)(&(this->loopback_addr)), MHD_OPTION_END);

        } else if (this->path_sslcert != "" && this->path_sslkey != "") {
            try {
                GetFileContent(this->path_sslcert, this->sslcert);
                GetFileContent(this->path_sslkey, this->sslkey);

                this->daemon = MHD_start_daemon(
                    MHD_USE_SSL | mhd_flags, this->port, NULL, NULL, (MHD_AccessHandlerCallback)HttpServer::callback,
                    this, MHD_OPTION_HTTPS_MEM_KEY, this->sslkey.c_str(), MHD_OPTION_HTTPS_MEM_CERT,
                    this->sslcert.c_str(), MHD_OPTION_THREAD_POOL_SIZE, this->threads, MHD_OPTION_END);
            } catch (JsonRpcException& ex) {
                return false;
            }
        } else {
            this->daemon =
                MHD_start_daemon(mhd_flags, this->port, NULL, NULL, (MHD_AccessHandlerCallback)HttpServer::callback,
                                 this, MHD_OPTION_THREAD_POOL_SIZE, this->threads, MHD_OPTION_END);
        }
        if (this->daemon != NULL)
            this->running = true;
    }
    return this->running;
}

bool HttpServer::StopListening() {
    if (this->running) {
        MHD_stop_daemon(this->daemon);
        this->running = false;
    }
    return true;
}

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

void HttpServer::SetUrlHandler(const string& url, ProtocolHandler* handler) {
    this->urlhandler[url] = handler;
    this->SetHandler(NULL);
}

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
            ProtocolHandler* handler = client_connection->server->GetHandler(string(url));
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

void HttpServer::ProcessRequest(const string& request, string& response) {
    if (this->handler != NULL) {
        this->handler->HandleRequest(request, response);
    }
}

void HttpServer::SetHandler(ProtocolHandler* handler) {
    this->handler = handler;
}

ProtocolHandler* HttpServer::GetHandler() {
    return this->handler;
}
