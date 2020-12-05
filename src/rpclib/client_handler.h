/*-----------------------------------------------------------------------
 * This file is part of TrueBlocks-RPC, which is a near-complete rewrite
 * of libjson-rpc-cpp removing anything not directly needed to support
 * an Ethereum RPC client and server. It retains the original license
 * as described in LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite 2021)
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

        string BuildRequest(const string& method, const jsonval_t& parameter);
        void BuildRequest(const string& method, const jsonval_t& parameter, string& result);
        void HandleResponse(const string& response, jsonval_t& result);
        jsonval_t HandleResponse(const jsonval_t& response, jsonval_t& result);

      private:
        void BuildRequest(int id, const string& method, const jsonval_t& parameter, jsonval_t& result);
        bool ValidateResponse(const jsonval_t& response);
        bool HasError(const jsonval_t& response);
        void throwErrorException(const jsonval_t& response);
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
