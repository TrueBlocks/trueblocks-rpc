/*-----------------------------------------------------------------------
 * This file was originally part of libjson-rpc-cpp which has been
 * almost completely re-written to remove anything not directly needed
 * by the Ethereum RPC. It retains the original license as described in
 * LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite circa 2020)
 *---------------------------------------------------------------------*/
#pragma once

#include <rpclib/procedure.h>

namespace jsonrpc {
    //---------------------------------------------------------------------------------------
    class Server_base {
      public:
        virtual ~Server_base() {
        }
        virtual void HandleMethodCall(Procedure& proc, const Json::Value& input, Json::Value& output) = 0;
    };
}  // namespace jsonrpc
