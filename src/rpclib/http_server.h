/*-----------------------------------------------------------------------
 * This file is part of TrueBlocks-RPC, which is a near-complete rewrite
 * of libjson-rpc-cpp removing anything not directly needed to support
 * an Ethereum RPC client and server. It retains the original license
 * as described in LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite 2021)
 *---------------------------------------------------------------------*/
#pragma once

#include <stdarg.h>
#include <stdint.h>
#include <sys/types.h>
#if defined(_WIN32) && !defined(__CYGWIN__)
#include <winsock2.h>
#include <ws2tcpip.h>
#if defined(_MSC_FULL_VER) && !defined(_SSIZE_T_DEFINED)
#define _SSIZE_T_DEFINED
typedef intptr_t ssize_t;
#endif  // !_SSIZE_T_DEFINED */
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#endif
#include <map>

#include <microhttpd.h>

#include <rpclib/server_handler.h>

namespace jsonrpc {
    //---------------------------------------------------------------------------------------
    class HttpServer {
      public:
        HttpServer(int port, const string& sslcert = "", const string& sslkey = "", int threads = 50);
        virtual ~HttpServer();
        HttpServer& BindLocalhost();

        virtual bool StartListening();
        virtual bool StopListening();

        bool virtual SendResponse(const string& response, void* addInfo = NULL);
        bool virtual SendOptionsResponse(void* addInfo);

        void SetUrlHandler(const string& url, ServerProtocolHandler* handler);
        void ProcessRequest(const string& request, string& response);
        void SetHandler(ServerProtocolHandler* handler);
        ServerProtocolHandler* GetHandler();

      private:
        ServerProtocolHandler* handler;

        int port;
        int threads;
        bool running;
        string path_sslcert;
        string path_sslkey;
        string sslcert;
        string sslkey;

        struct MHD_Daemon* daemon;
        bool bindlocalhost;
        std::map<string, ServerProtocolHandler*> urlhandler;
        struct sockaddr_in loopback_addr;

        static int callback(void* cls, struct MHD_Connection* connection, const char* url, const char* method,
                            const char* version, const char* upload_data, size_t* upload_data_size, void** con_cls);

        ServerProtocolHandler* GetHandler(const string& url);
    };

} /* namespace jsonrpc */
