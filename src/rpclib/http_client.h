/*-----------------------------------------------------------------------
 * This file was originally part of libjson-rpc-cpp which has been
 * almost completely re-written to remove anything not directly needed
 * by the Ethereum RPC. It retains the original license as described in
 * LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite circa 2020)
 *---------------------------------------------------------------------*/
#pragma once

#include <map>
#include <curl/curl.h>

#include <rpclib/exception.h>

namespace jsonrpc {
    //---------------------------------------------------------------------------------------
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
        long timeout;
        CURL* curl;
    };

} /* namespace jsonrpc */
