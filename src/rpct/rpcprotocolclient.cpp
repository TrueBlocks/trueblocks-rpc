/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    responsehandler.cpp
 * @date    13.03.2013
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include <rpct/rpcprotocolclient.h>

#include <json/json.h>

using namespace std;
using namespace jsonrpc;

const string RpcProtocolClient::KEY_PROTOCOL_VERSION = "jsonrpc";
const string RpcProtocolClient::KEY_PROCEDURE_NAME = "method";
const string RpcProtocolClient::KEY_ID = "id";
const string RpcProtocolClient::KEY_PARAMETER = "params";
const string RpcProtocolClient::KEY_AUTH = "auth";
const string RpcProtocolClient::KEY_RESULT = "result";
const string RpcProtocolClient::KEY_ERROR = "error";
const string RpcProtocolClient::KEY_ERROR_CODE = "code";
const string RpcProtocolClient::KEY_ERROR_MESSAGE = "message";
const string RpcProtocolClient::KEY_ERROR_DATA = "data";

RpcProtocolClient::RpcProtocolClient(bool omitEndingLineFeed) : omitEndingLineFeed(omitEndingLineFeed) {
}

void RpcProtocolClient::BuildRequest(const string& method, const Json::Value& parameter, string& result,
                                     bool isNotification) {
    Json::Value request;
    Json::StreamWriterBuilder wbuilder;
    wbuilder["indentation"] = "";
    this->BuildRequest(1, method, parameter, request, isNotification);

    result = Json::writeString(wbuilder, request);
}

void RpcProtocolClient::HandleResponse(const string& response, Json::Value& result) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    Json::Reader reader;
#pragma clang diagnostic pop
    Json::Value value;

    try {
        if (reader.parse(response, value)) {
            this->HandleResponse(value, result);
        } else {
            throw JsonRpcException(Errors::ERROR_RPC_JSON_PARSE_ERROR, " " + response);
        }
    } catch (Json::Exception& e) {
        throw JsonRpcException(Errors::ERROR_RPC_JSON_PARSE_ERROR, " " + response);
    }
}

Json::Value RpcProtocolClient::HandleResponse(const Json::Value& value, Json::Value& result) {
    if (this->ValidateResponse(value)) {
        if (this->HasError(value)) {
            this->throwErrorException(value);
        } else {
            result = value[KEY_RESULT];
        }
    } else {
        throw JsonRpcException(Errors::ERROR_CLIENT_INVALID_RESPONSE, " " + value.toStyledString());
    }
    return value[KEY_ID];
}

void RpcProtocolClient::BuildRequest(int id, const string& method, const Json::Value& parameter, Json::Value& result,
                                     bool isNotification) {
    result[KEY_PROTOCOL_VERSION] = "2.0";
    result[KEY_PROCEDURE_NAME] = method;
    if (parameter != Json::nullValue)
        result[KEY_PARAMETER] = parameter;
    if (!isNotification)
        result[KEY_ID] = id;
}

void RpcProtocolClient::throwErrorException(const Json::Value& response) {
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

bool RpcProtocolClient::ValidateResponse(const Json::Value& response) {
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

bool RpcProtocolClient::HasError(const Json::Value& response) {
    return response.isMember(KEY_ERROR);
}
