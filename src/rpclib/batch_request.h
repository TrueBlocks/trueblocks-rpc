/*-----------------------------------------------------------------------
 * This file is part of TrueBlocks-RPC, which is a near-complete rewrite
 * of libjson-rpc-cpp removing anything not directly needed to support
 * an Ethereum RPC client and server. It retains the original license
 * as described in LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite 2021)
 *---------------------------------------------------------------------*/
#pragma once

#include <json/json.h>

#include <rpclib/procedure.h>

namespace jsonrpc {
    //---------------------------------------------------------------------------------------
    class BatchRequest {
      public:
        BatchRequest();
        int addCall(const string& methodname, const jsonval_t& params);
        string toString(bool fast = true) const;

      private:
        jsonval_t result;
        int id;
    };
}  // namespace jsonrpc
