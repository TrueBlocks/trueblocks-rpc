/*-----------------------------------------------------------------------
 * This file is part of TrueBlocks-RPC, which is a near-complete rewrite
 * of libjson-rpc-cpp removing anything not directly needed to support
 * an Ethereum RPC client and server. It retains the original license
 * as described in LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite 2021)
 *---------------------------------------------------------------------*/
#pragma once

#include <rpclib/procedure.h>

namespace jsonrpc {
    //---------------------------------------------------------------------------------------
    class Server_base {
      public:
        virtual ~Server_base() {
        }
        virtual void HandleMethodCall(Procedure& proc, const jsonval_t& input, jsonval_t& output) = 0;
    };
}  // namespace jsonrpc
