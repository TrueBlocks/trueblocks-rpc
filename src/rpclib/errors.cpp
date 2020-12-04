/*-----------------------------------------------------------------------
 * This file was originally part of libjson-rpc-cpp which has been
 * almost completely re-written to remove anything not directly needed
 * by the Ethereum RPC. It retains the original license as described in
 * LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite circa 2020)
 *---------------------------------------------------------------------*/
#include <rpclib/errors.h>
#include <rpclib/exception.h>

using namespace jsonrpc;

std::map<int, string> Errors::possibleErrors;
Errors::_init Errors::_initializer;

const int Errors::ERROR_RPC_JSON_PARSE_ERROR = -32700;
const int Errors::ERROR_RPC_METHOD_NOT_FOUND = -32601;
const int Errors::ERROR_RPC_INVALID_REQUEST = -32600;
const int Errors::ERROR_RPC_INVALID_PARAMS = -32602;
const int Errors::ERROR_RPC_INTERNAL_ERROR = -32603;

const int Errors::ERROR_SERVER_PROCEDURE_POINTER_IS_NULL = -32606;
const int Errors::ERROR_SERVER_PROCEDURE_SPECIFICATION_NOT_FOUND = -32000;
const int Errors::ERROR_SERVER_CONNECTOR = -32002;
const int Errors::ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX = -32007;

const int Errors::ERROR_CLIENT_CONNECTOR = -32003;
const int Errors::ERROR_CLIENT_INVALID_RESPONSE = -32001;

const int Errors::TG_ERROR_SERVER_DEPRECATED = -132001;
const int Errors::TG_ERROR_SERVER_NOTIMPLEMENTED = -132002;

Errors::_init::_init() {
    // Official Errors
    possibleErrors[ERROR_RPC_INVALID_REQUEST] =
        "INVALID_JSON_REQUEST: The JSON "
        "sent is not a valid JSON-RPC "
        "Request object";
    possibleErrors[ERROR_RPC_METHOD_NOT_FOUND] =
        "METHOD_NOT_FOUND: The method "
        "being requested is not "
        "available on this server";
    possibleErrors[ERROR_RPC_INVALID_PARAMS] =
        "INVALID_PARAMS: Invalid method "
        "parameters (invalid name and/or "
        "type) recognised";
    possibleErrors[ERROR_RPC_JSON_PARSE_ERROR] = "JSON_PARSE_ERROR: The JSON-Object is not JSON-Valid";
    possibleErrors[ERROR_RPC_INTERNAL_ERROR] = "INTERNAL_ERROR: ";

    possibleErrors[ERROR_SERVER_PROCEDURE_POINTER_IS_NULL] =
        "PROCEDURE_POINTER_IS_NULL: Server has no function Reference registered";
    possibleErrors[ERROR_SERVER_PROCEDURE_SPECIFICATION_NOT_FOUND] = "Configuration file was not found";

    possibleErrors[ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX] = "Procedure specification mallformed";

    possibleErrors[ERROR_CLIENT_INVALID_RESPONSE] = "The response is invalid";
    possibleErrors[ERROR_CLIENT_CONNECTOR] = "Client connector error";
    possibleErrors[ERROR_SERVER_CONNECTOR] = "Server connector error";

    possibleErrors[TG_ERROR_SERVER_DEPRECATED] = "the method has been deprecated";
    possibleErrors[TG_ERROR_SERVER_NOTIMPLEMENTED] = "the method is currently not implemented";
}

string Errors::GetErrorMessage(int errorCode) {
    if (possibleErrors.find(errorCode) == possibleErrors.end()) {
        return "";
    }
    return possibleErrors[errorCode];
}
