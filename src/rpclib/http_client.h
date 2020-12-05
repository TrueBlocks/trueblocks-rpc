/*-----------------------------------------------------------------------
 * This file is part of TrueBlocks-RPC, which is a near-complete rewrite
 * of libjson-rpc-cpp removing anything not directly needed to support
 * an Ethereum RPC client and server. It retains the original license
 * as described in LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite 2021)
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
