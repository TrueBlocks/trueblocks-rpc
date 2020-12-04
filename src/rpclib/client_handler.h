/*-----------------------------------------------------------------------
 * This file was originally part of libjson-rpc-cpp which has been
 * almost completely re-written to remove anything not directly needed
 * by the Ethereum RPC. It retains the original license as described in
 * LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite circa 2020)
 *---------------------------------------------------------------------*/
#pragma once

#include <string>
#include <json/json.h>

#include <rpclib/exception.h>
#include <rpclib/client.h>

namespace jsonrpc {
    //---------------------------------------------------------------------------------------
    class ClientProtocolHandler {
      public:
        ClientProtocolHandler();

        string BuildRequest(const string& method, const Json::Value& parameter);
        void BuildRequest(const string& method, const Json::Value& parameter, string& result);
        void HandleResponse(const string& response, Json::Value& result);
        Json::Value HandleResponse(const Json::Value& response, Json::Value& result);

      private:
        void BuildRequest(int id, const string& method, const Json::Value& parameter, Json::Value& result);
        bool ValidateResponse(const Json::Value& response);
        bool HasError(const Json::Value& response);
        void throwErrorException(const Json::Value& response);
    };

    extern const char* KEY_PROTOCOL_VERSION;
    extern const char* KEY_PROCEDURE_NAME;
    extern const char* KEY_ID;
    extern const char* KEY_PARAMETER;
    extern const char* KEY_AUTH;
    extern const char* KEY_RESULT;
    extern const char* KEY_ERROR;
    extern const char* KEY_ERROR_CODE;
    extern const char* KEY_ERROR_MESSAGE;
    extern const char* KEY_ERROR_DATA;
}  // namespace jsonrpc
