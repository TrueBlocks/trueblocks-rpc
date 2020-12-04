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
#include <vector>
#include <json/json.h>

#include <rpclib/batch_request.h>
#include <rpclib/batch_response.h>
#include <rpclib/http_client.h>

namespace jsonrpc {
    class ClientProtocolHandler;
    //---------------------------------------------------------------------------------------
    class Client {
      public:
        Client(HttpClient& connector);
        virtual ~Client();

        void CallMethod(const string& name, const Json::Value& parameter, Json::Value& result);
        Json::Value CallMethod(const string& name, const Json::Value& parameter);

        void CallProcedures(const BatchRequest& calls, BatchResponse& response);
        BatchResponse CallProcedures(const BatchRequest& calls);

      private:
        HttpClient& connector;
        ClientProtocolHandler* protocol;
    };

} /* namespace jsonrpc */
