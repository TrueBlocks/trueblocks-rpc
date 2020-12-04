/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    iprocedureinvokationhandler.h
 * @date    23.10.2014
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/
#pragma once

#include <rpclib/procedure.h>

namespace jsonrpc {
    class IProcedureInvokationHandler {
      public:
        virtual ~IProcedureInvokationHandler() {
        }
        virtual void HandleMethodCall(Procedure& proc, const Json::Value& input, Json::Value& output) = 0;
    };
}  // namespace jsonrpc
