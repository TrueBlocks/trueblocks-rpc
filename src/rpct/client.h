/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    client.h
 * @date    03.01.2013
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/
#pragma once

#include <json/json.h>

#include <map>
#include <vector>

#include "batchcall.h"
#include "batchresponse.h"
#include "httpclient.h"

namespace jsonrpc {
    class ClientProtocolHandler;

    class Client {
      public:
        Client(HttpClient& connector, bool omitEndingLineFeed = false);
        virtual ~Client();

        void CallMethod(const string& name, const Json::Value& parameter, Json::Value& result);
        Json::Value CallMethod(const string& name, const Json::Value& parameter);

        void CallProcedures(const BatchCall& calls, BatchResponse& response);
        BatchResponse CallProcedures(const BatchCall& calls);

        void CallNotification(const string& name, const Json::Value& parameter);

      private:
        HttpClient& connector;
        ClientProtocolHandler* protocol;
    };

} /* namespace jsonrpc */
