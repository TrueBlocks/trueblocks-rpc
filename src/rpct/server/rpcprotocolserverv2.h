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

#include <rpct/libs/exception.h>
#include "rpct/server/iprocedureinvokationhandler.h"

namespace jsonrpc {
    class ProtocolHandler {
      public:
        ProtocolHandler(IProcedureInvokationHandler& handler);

        void HandleJsonRequest(const Json::Value& request, Json::Value& response);
        void HandleRequest(const string& request, string& retValue);
        void AddProcedure(const Procedure& procedure);

      private:
        bool ValidateRequestFields(const Json::Value& val);
        void WrapResult(const Json::Value& request, Json::Value& response, Json::Value& retValue);
        void WrapError(const Json::Value& request, int code, const string& message, Json::Value& result);
        void WrapException(const Json::Value& request, const JsonRpcException& exception, Json::Value& result);
        procedure_t GetRequestType(const Json::Value& request);
        void ProcessRequest(const Json::Value& request, Json::Value& retValue);
        int ValidateRequest(const Json::Value& val);

        IProcedureInvokationHandler& handler;
        std::map<string, Procedure> procedures;
        void HandleSingleRequest(const Json::Value& request, Json::Value& response);
        void HandleBatchRequest(const Json::Value& requests, Json::Value& response);
        // no copies
        ProtocolHandler(const ProtocolHandler& c) : handler(c.handler){};  // no copy
        ProtocolHandler& operator=(const ProtocolHandler&) {
            return *this;
        }  // no assignment
    };

} /* namespace jsonrpc */
#endif /* JSONRPC_CPP_RPCPROTOCOLSERVERV2_H_ */
