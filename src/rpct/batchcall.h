/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    batchcall.h
 * @date    15.10.2013
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/
#pragma once

#include <json/json.h>
#include <rpct/procedure.h>

namespace jsonrpc {
    class BatchCall {
      public:
        BatchCall();

        /**
         * @brief addCall
         * @param methodname
         * @param params
         * @param isNotification
         * @return the id of the geneared request inside the batchcall
         */
        int addCall(const string& methodname, const Json::Value& params, bool isNotification = false);
        string toString(bool fast = true) const;

      private:
        Json::Value result;
        int id;
    };
}  // namespace jsonrpc
