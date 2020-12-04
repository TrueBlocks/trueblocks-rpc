/*-----------------------------------------------------------------------
 * This file was originally part of libjson-rpc-cpp which has been almost
 * entirely re-written to remove anything not directly needed by the Ethereum
 * RPC. It retains the original license as described in LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite circa 2021)
 *---------------------------------------------------------------------*/
#pragma once

#include <map>
#include <string>
#include <json/json.h>

#include <rpclib/utils.h>

namespace jsonrpc {
    class JsonRpcException;

    //---------------------------------------------------------------------------------------
    class Errors {
      public:
        static class _init {
          public:
            _init();
        } _initializer;
        static string GetErrorMessage(int errorCode);

      private:
        static std::map<int, string> possibleErrors;
    };

    extern const int ERROR_RPC_JSON_PARSE_ERROR;
    extern const int ERROR_RPC_METHOD_NOT_FOUND;
    extern const int ERROR_RPC_INVALID_REQUEST;
    extern const int ERROR_RPC_INVALID_PARAMS;
    extern const int ERROR_RPC_INTERNAL_ERROR;

    extern const int ERROR_SERVER_PROCEDURE_POINTER_IS_NULL;
    extern const int ERROR_SERVER_PROCEDURE_SPECIFICATION_NOT_FOUND;
    extern const int ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX;
    extern const int ERROR_SERVER_CONNECTOR;

    extern const int ERROR_CLIENT_CONNECTOR;
    extern const int ERROR_CLIENT_INVALID_RESPONSE;

    extern const int TG_ERROR_SERVER_DEPRECATED;
    extern const int TG_ERROR_SERVER_NOTIMPLEMENTED;
} /* namespace jsonrpc */
