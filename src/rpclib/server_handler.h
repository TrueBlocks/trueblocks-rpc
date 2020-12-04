/*-----------------------------------------------------------------------
 * This file was originally part of libjson-rpc-cpp which has been
 * almost completely re-written to remove anything not directly needed
 * by the Ethereum RPC. It retains the original license as described in
 * LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite circa 2020)
 *---------------------------------------------------------------------*/
#pragma once

#include <string>
#include <vector>
#include <map>

#include <rpclib/exception.h>
#include <rpclib/procedure.h>
#include <rpclib/server_base.h>

namespace jsonrpc {

    class ServerProtocolHandler {
      public:
        ServerProtocolHandler(Server_base& handler);

        void HandleJsonRequest(const Json::Value& request, Json::Value& response);
        void HandleRequest(const string& request, string& retValue);
        void AddProcedure(const Procedure& procedure);

      private:
        bool ValidateRequestFields(const Json::Value& val);
        void WrapResult(const Json::Value& request, Json::Value& response, Json::Value& retValue);
        void WrapError(const Json::Value& request, int code, const string& message, Json::Value& result);
        void WrapException(const Json::Value& request, const JsonRpcException& exception, Json::Value& result);
        void ProcessRequest(const Json::Value& request, Json::Value& retValue);
        int ValidateRequest(const Json::Value& val);

        Server_base& handler;
        std::map<string, Procedure> procedures;
        void HandleSingleRequest(const Json::Value& request, Json::Value& response);
        void HandleBatchRequest(const Json::Value& requests, Json::Value& response);
        // no copies
        ServerProtocolHandler(const ServerProtocolHandler& c) : handler(c.handler){};  // no copy
        ServerProtocolHandler& operator=(const ServerProtocolHandler&) {
            return *this;
        }  // no assignment
    };

} /* namespace jsonrpc */
