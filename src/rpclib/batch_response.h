/*-----------------------------------------------------------------------
 * This file was originally part of libjson-rpc-cpp which has been
 * almost completely re-written to remove anything not directly needed
 * by the Ethereum RPC. It retains the original license as described in
 * LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite circa 2020)
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

        void addResponse(Json::Value& id, Json::Value response, bool isError = false);
        Json::Value getResult(int id);
        void getResult(Json::Value& id, Json::Value& result);
        int getErrorCode(Json::Value& id);
        string getErrorMessage(Json::Value& id);
        string getErrorMessage(int id);
        bool hasErrors();

      private:
        std::map<Json::Value, Json::Value> responses;
        std::vector<Json::Value> errorResponses;
    };

}  // namespace jsonrpc
