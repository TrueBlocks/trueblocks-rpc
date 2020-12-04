/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    rpcprotocolserverv2.h
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef JSONRPC_CPP_RPCPROTOCOLSERVERV2_H_
#define JSONRPC_CPP_RPCPROTOCOLSERVERV2_H_

#include <string>
#include <vector>
#include <map>

#include <rpct/exception.h>
#include "rpct/iprocedureinvokationhandler.h"

namespace jsonrpc {
    class ServerProtocolHandler {
      public:
        ServerProtocolHandler(IProcedureInvokationHandler& handler);

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

        IProcedureInvokationHandler& handler;
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
#endif /* JSONRPC_CPP_RPCPROTOCOLSERVERV2_H_ */
