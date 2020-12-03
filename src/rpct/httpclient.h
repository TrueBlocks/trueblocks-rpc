/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpclient.h
 * @date    02.01.2013
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef JSONRPC_CPP_HTTPCLIENT_H_
#define JSONRPC_CPP_HTTPCLIENT_H_

#include <curl/curl.h>
#include <rpct/exception.h>
#include <map>

namespace jsonrpc {
    class HttpClient {
      public:
        HttpClient(const string& url);
        virtual ~HttpClient();
        virtual void SendRPCMessage(const string& message, string& result);

        void SetUrl(const string& url);
        void SetTimeout(long timeout);

        void AddHeader(const string& attr, const string& val);
        void RemoveHeader(const string& attr);

      private:
        std::map<string, string> headers;
        string url;

        /**
         * @brief timeout for http request in milliseconds
         */
        long timeout;
        CURL* curl;
    };

} /* namespace jsonrpc */
#endif /* JSONRPC_CPP_HTTPCLIENT_H_ */
