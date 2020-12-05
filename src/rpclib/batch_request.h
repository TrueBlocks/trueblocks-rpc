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

        /**
         * @brief addCall
         * @param methodname
         * @param params
         * @return the id of the geneared request inside the BatchRequest
         */
        int addCall(const string& methodname, const Json::Value& params);
        string toString(bool fast = true) const;

      private:
        Json::Value result;
        int id;
    };
}  // namespace jsonrpc
