/*-----------------------------------------------------------------------
 * This file is part of TrueBlocks-RPC, which is a near-complete rewrite
 * of libjson-rpc-cpp removing anything not directly needed to support
 * an Ethereum RPC client and server. It retains the original license
 * as described in LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite 2021)
 *---------------------------------------------------------------------*/
#pragma once

#include <map>
#include <json/json.h>

#include <rpclib/utils.h>

namespace jsonrpc {
    //---------------------------------------------------------------------------------------
    class BatchResponse {
      public:
        BatchResponse();

        void addResponse(jsonval_t& id, jsonval_t response, bool isError = false);
        jsonval_t getResult(int id);
        void getResult(jsonval_t& id, jsonval_t& result);
        int getErrorCode(jsonval_t& id);
        string getErrorMessage(jsonval_t& id);
        string getErrorMessage(int id);
        bool hasErrors();

      private:
        std::map<jsonval_t, jsonval_t> responses;
        std::vector<jsonval_t> errorResponses;
    };

}  // namespace jsonrpc
