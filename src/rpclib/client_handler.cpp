/*-----------------------------------------------------------------------
 * This file is part of TrueBlocks-RPC, which is a near-complete rewrite
 * of libjson-rpc-cpp removing anything not directly needed to support
 * an Ethereum RPC client and server. It retains the original license
 * as described in LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite 2021)
 *---------------------------------------------------------------------*/
#include <json/json.h>

#include <rpclib/client_handler.h>

using namespace std;
using namespace jsonrpc;

//---------------------------------------------------------------------------------------
namespace jsonrpc {
    const char* KEY_PROTOCOL_VERSION = "jsonrpc";
    const char* KEY_PROCEDURE_NAME = "method";
    const char* KEY_ID = "id";
    const char* KEY_PARAMETER = "params";
    const char* KEY_AUTH = "auth";
    const char* KEY_RESULT = "result";
    const char* KEY_ERROR = "error";
    const char* KEY_ERROR_CODE = "code";
    const char* KEY_ERROR_MESSAGE = "message";
    const char* KEY_ERROR_DATA = "data";
}  // namespace jsonrpc

//---------------------------------------------------------------------------------------
ClientProtocolHandler::ClientProtocolHandler() {
}

//---------------------------------------------------------------------------------------
void ClientProtocolHandler::BuildRequest(const string& method, const jsonval_t& parameter, string& result) {
    jsonval_t request;
    Json::StreamWriterBuilder wbuilder;
    wbuilder["indLevel"] = "";
    BuildRequest(1, method, parameter, request);
    result = Json::writeString(wbuilder, request);
}

//---------------------------------------------------------------------------------------
void ClientProtocolHandler::HandleResponse(const string& response, jsonval_t& result) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    Json::Reader reader;
#pragma clang diagnostic pop
    jsonval_t value;

    try {
        if (reader.parse(response, value)) {
            HandleResponse(value, result);
        } else {
            throw JsonRpcException(ERROR_RPC_JSON_PARSE_ERROR, " " + response);
        }
    } catch (Json::Exception& e) {
        throw JsonRpcException(ERROR_RPC_JSON_PARSE_ERROR, " " + response);
    }
}

//---------------------------------------------------------------------------------------
jsonval_t ClientProtocolHandler::HandleResponse(const jsonval_t& value, jsonval_t& result) {
    if (ValidateResponse(value)) {
        if (HasError(value)) {
            throwErrorException(value);
        } else {
            result = value[KEY_RESULT];
        }
    } else {
        throw JsonRpcException(ERROR_CLIENT_INVALID_RESPONSE, " " + value.toStyledString());
    }
    return value[KEY_ID];
}

//---------------------------------------------------------------------------------------
void ClientProtocolHandler::BuildRequest(int id, const string& method, const jsonval_t& parameter, jsonval_t& result) {
    result[KEY_PROTOCOL_VERSION] = "2.0";
    result[KEY_PROCEDURE_NAME] = method;
    if (parameter != Json::nullValue)
        result[KEY_PARAMETER] = parameter;
    result[KEY_ID] = id;
}

//---------------------------------------------------------------------------------------
void ClientProtocolHandler::throwErrorException(const jsonval_t& response) {
    if (response[KEY_ERROR].isMember(KEY_ERROR_MESSAGE) && response[KEY_ERROR][KEY_ERROR_MESSAGE].isString()) {
        if (response[KEY_ERROR].isMember(KEY_ERROR_DATA)) {
            throw JsonRpcException(response[KEY_ERROR][KEY_ERROR_CODE].asInt(),
                                   response[KEY_ERROR][KEY_ERROR_MESSAGE].asString(),
                                   response[KEY_ERROR][KEY_ERROR_DATA]);
        } else {
            throw JsonRpcException(response[KEY_ERROR][KEY_ERROR_CODE].asInt(),
                                   response[KEY_ERROR][KEY_ERROR_MESSAGE].asString());
        }
    } else {
        throw JsonRpcException(response[KEY_ERROR][KEY_ERROR_CODE].asInt());
    }
}

//---------------------------------------------------------------------------------------
bool ClientProtocolHandler::ValidateResponse(const jsonval_t& response) {
    if (!response.isObject() || !response.isMember(KEY_ID))
        return false;

    if (!response.isMember(KEY_PROTOCOL_VERSION) || response[KEY_PROTOCOL_VERSION] != "2.0")
        return false;

    if (response.isMember(KEY_RESULT) && response.isMember(KEY_ERROR))
        return false;

    if (!response.isMember(KEY_RESULT) && !response.isMember(KEY_ERROR))
        return false;

    if (response.isMember(KEY_ERROR) &&
        !(response[KEY_ERROR].isObject() && response[KEY_ERROR].isMember(KEY_ERROR_CODE) &&
          response[KEY_ERROR][KEY_ERROR_CODE].isIntegral()))
        return false;

    return true;
}

//---------------------------------------------------------------------------------------
bool ClientProtocolHandler::HasError(const jsonval_t& response) {
    return response.isMember(KEY_ERROR);
}
