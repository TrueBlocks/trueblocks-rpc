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

        void HandleJsonRequest(const jsonval_t& request, jsonval_t& response);
        void HandleRequest(const string& request, string& retValue);
        void AddProcedure(const Procedure& procedure);

      private:
        void ProcessRequest(const jsonval_t& request, jsonval_t& retValue);
        void HandleSingleRequest(const jsonval_t& request, jsonval_t& response);
        void HandleBatchRequest(const jsonval_t& requests, jsonval_t& response);

        bool ValidateRequestFields(const jsonval_t& val);
        int ValidateRequest(const jsonval_t& val);

        void WrapResult(const jsonval_t& request, jsonval_t& response, jsonval_t& retValue);
        void WrapError(const jsonval_t& request, int code, const string& message, jsonval_t& result);
        void WrapException(const jsonval_t& request, const JsonRpcException& exception, jsonval_t& result);

        Server_base& handler;
        std::map<string, Procedure> procedures;

      private:  // no copies
        ServerProtocolHandler(const ServerProtocolHandler& c) : handler(c.handler){};
        ServerProtocolHandler& operator=(const ServerProtocolHandler&);
    };

} /* namespace jsonrpc */
