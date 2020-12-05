/*-----------------------------------------------------------------------
 * This file is part of TrueBlocks-RPC, which is a near-complete rewrite
 * of libjson-rpc-cpp removing anything not directly needed to support
 * an Ethereum RPC client and server. It retains the original license
 * as described in LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite 2021)
 *---------------------------------------------------------------------*/
#pragma once

#include <string>
#include <vector>
#include <map>

#include <rpclib/exception.h>
#include <rpclib/procedure.h>
#include <rpclib/server_base.h>

namespace jsonrpc {

    //---------------------------------------------------------------------------------------
    class ServerProtocolHandler {
      public:
        ServerProtocolHandler(Server_base& handler);

        void HandleJsonRequest(const Json::Value& request, Json::Value& response);
        void HandleRequest(const string& request, string& retValue);
        void AddProcedure(const Procedure& procedure);

      private:
        void ProcessRequest(const Json::Value& request, Json::Value& retValue);
        void HandleSingleRequest(const Json::Value& request, Json::Value& response);
        void HandleBatchRequest(const Json::Value& requests, Json::Value& response);

        bool ValidateRequestFields(const Json::Value& val);
        int ValidateRequest(const Json::Value& val);

        void WrapResult(const Json::Value& request, Json::Value& response, Json::Value& retValue);
        void WrapError(const Json::Value& request, int code, const string& message, Json::Value& result);
        void WrapException(const Json::Value& request, const JsonRpcException& exception, Json::Value& result);

        Server_base& handler;
        std::map<string, Procedure> procedures;

      private:  // no copies
        ServerProtocolHandler(const ServerProtocolHandler& c) : handler(c.handler){};
        ServerProtocolHandler& operator=(const ServerProtocolHandler&);
    };

} /* namespace jsonrpc */
