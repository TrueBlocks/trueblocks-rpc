/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    responsehandler.cpp
 * @date    13.03.2013
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include <rpct/clientprotocolhandler.h>

#include <json/json.h>

using namespace std;
using namespace jsonrpc;

const string ClientProtocolHandler::KEY_PROTOCOL_VERSION = "jsonrpc";
const string ClientProtocolHandler::KEY_PROCEDURE_NAME = "method";
const string ClientProtocolHandler::KEY_ID = "id";
const string ClientProtocolHandler::KEY_PARAMETER = "params";
const string ClientProtocolHandler::KEY_AUTH = "auth";
const string ClientProtocolHandler::KEY_RESULT = "result";
const string ClientProtocolHandler::KEY_ERROR = "error";
const string ClientProtocolHandler::KEY_ERROR_CODE = "code";
const string ClientProtocolHandler::KEY_ERROR_MESSAGE = "message";
const string ClientProtocolHandler::KEY_ERROR_DATA = "data";

ClientProtocolHandler::ClientProtocolHandler(bool omitEndingLineFeed) : omitEndingLineFeed(omitEndingLineFeed) {
}

void ClientProtocolHandler::BuildRequest(const string& method, const Json::Value& parameter, string& result,
                                         bool isNotification) {
    Json::Value request;
    Json::StreamWriterBuilder wbuilder;
    wbuilder["indentation"] = "";
    BuildRequest(1, method, parameter, request, isNotification);

    result = Json::writeString(wbuilder, request);
}

void ClientProtocolHandler::HandleResponse(const string& response, Json::Value& result) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    Json::Reader reader;
#pragma clang diagnostic pop
    Json::Value value;

    try {
        if (reader.parse(response, value)) {
            HandleResponse(value, result);
        } else {
            throw JsonRpcException(Errors::ERROR_RPC_JSON_PARSE_ERROR, " " + response);
        }
    } catch (Json::Exception& e) {
        throw JsonRpcException(Errors::ERROR_RPC_JSON_PARSE_ERROR, " " + response);
    }
}

Json::Value ClientProtocolHandler::HandleResponse(const Json::Value& value, Json::Value& result) {
    if (ValidateResponse(value)) {
        if (HasError(value)) {
            throwErrorException(value);
        } else {
            result = value[KEY_RESULT];
        }
    } else {
        throw JsonRpcException(Errors::ERROR_CLIENT_INVALID_RESPONSE, " " + value.toStyledString());
    }
    return value[KEY_ID];
}

void ClientProtocolHandler::BuildRequest(int id, const string& method, const Json::Value& parameter,
                                         Json::Value& result, bool isNotification) {
    result[KEY_PROTOCOL_VERSION] = "2.0";
    result[KEY_PROCEDURE_NAME] = method;
    if (parameter != Json::nullValue)
        result[KEY_PARAMETER] = parameter;
    if (!isNotification)
        result[KEY_ID] = id;
}

void ClientProtocolHandler::throwErrorException(const Json::Value& response) {
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

bool ClientProtocolHandler::ValidateResponse(const Json::Value& response) {
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

bool ClientProtocolHandler::HasError(const Json::Value& response) {
    return response.isMember(KEY_ERROR);
}
