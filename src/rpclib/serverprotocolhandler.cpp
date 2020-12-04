/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    rpcprotocolserverv2.cpp
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include <rpclib/serverprotocolhandler.h>
#include <rpclib/errors.h>
#include <iostream>

using namespace std;
using namespace jsonrpc;

ServerProtocolHandler::ServerProtocolHandler(IProcedureInvokationHandler& h) : handler(h) {
}

void ServerProtocolHandler::HandleJsonRequest(const Json::Value& req, Json::Value& response) {
    // It could be a Batch Request
    if (req.isArray()) {
        HandleBatchRequest(req, response);
    }  // It could be a simple Request
    else if (req.isObject()) {
        HandleSingleRequest(req, response);
    } else {
        WrapError(Json::nullValue, Errors::ERROR_RPC_INVALID_REQUEST,
                  Errors::GetErrorMessage(Errors::ERROR_RPC_INVALID_REQUEST), response);
    }
}
void ServerProtocolHandler::HandleSingleRequest(const Json::Value& req, Json::Value& response) {
    int error = ValidateRequest(req);
    if (error == 0) {
        try {
            ProcessRequest(req, response);
        } catch (const JsonRpcException& exc) {
            WrapException(req, exc, response);
        }
    } else {
        WrapError(req, error, Errors::GetErrorMessage(error), response);
    }
}
void ServerProtocolHandler::HandleBatchRequest(const Json::Value& req, Json::Value& response) {
    if (req.size() == 0)
        WrapError(Json::nullValue, Errors::ERROR_RPC_INVALID_REQUEST,
                  Errors::GetErrorMessage(Errors::ERROR_RPC_INVALID_REQUEST), response);
    else {
        for (unsigned int i = 0; i < req.size(); i++) {
            Json::Value result;
            HandleSingleRequest(req[i], result);
            if (result != Json::nullValue)
                response.append(result);
        }
    }
}

extern const char* KEY_REQUEST_METHODNAME;
extern const char* KEY_REQUEST_ID;
extern const char* KEY_REQUEST_PARAMETERS;
extern const char* KEY_RESPONSE_ERROR;
extern const char* KEY_RESPONSE_RESULT;
extern const char* KEY_REQUEST_VERSION;
extern const char* JSON_RPC_VERSION2;

bool ServerProtocolHandler::ValidateRequestFields(const Json::Value& request) {
    if (!request.isObject())
        return false;
    if (!(request.isMember(KEY_REQUEST_METHODNAME) && request[KEY_REQUEST_METHODNAME].isString()))
        return false;
    if (!(request.isMember(KEY_REQUEST_VERSION) && request[KEY_REQUEST_VERSION].isString() &&
          request[KEY_REQUEST_VERSION].asString() == JSON_RPC_VERSION2))
        return false;
    if (request.isMember(KEY_REQUEST_ID) && !(request[KEY_REQUEST_ID].isIntegral() ||
                                              request[KEY_REQUEST_ID].isString() || request[KEY_REQUEST_ID].isNull()))
        return false;
    if (request.isMember(KEY_REQUEST_PARAMETERS) &&
        !(request[KEY_REQUEST_PARAMETERS].isObject() || request[KEY_REQUEST_PARAMETERS].isArray() ||
          request[KEY_REQUEST_PARAMETERS].isNull()))
        return false;
    return true;
}

void ServerProtocolHandler::WrapResult(const Json::Value& request, Json::Value& response, Json::Value& result) {
    response[KEY_REQUEST_VERSION] = JSON_RPC_VERSION2;
    response[KEY_RESPONSE_RESULT] = result;
    response[KEY_REQUEST_ID] = request[KEY_REQUEST_ID];
}

void ServerProtocolHandler::WrapError(const Json::Value& request, int code, const string& message,
                                      Json::Value& result) {
    result["jsonrpc"] = "2.0";
    if (code == Errors::TG_ERROR_SERVER_DEPRECATED || code == Errors::TG_ERROR_SERVER_NOTIMPLEMENTED)
        code = -32000;  // why? because possibleErrors is indexed by error code, but more than one error code has
                        // different message
    result["error"]["code"] = code;
    result["error"]["message"] = message;

    if (request.isObject() && request.isMember("id") &&
        (request["id"].isNull() || request["id"].isIntegral() || request["id"].isString())) {
        result["id"] = request["id"];
    } else {
        result["id"] = Json::nullValue;
    }
}

void ServerProtocolHandler::WrapException(const Json::Value& request, const JsonRpcException& exception,
                                          Json::Value& result) {
    WrapError(request, exception.GetCode(), exception.GetMessage(), result);
    result["error"]["data"] = exception.GetData();
}

void ServerProtocolHandler::AddProcedure(const Procedure& procedure) {
    procedures[procedure.GetProcedureName()] = procedure;
}

void ServerProtocolHandler::HandleRequest(const string& request, string& retValue) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    Json::Reader reader;
#pragma clang diagnostic pop
    Json::Value req;
    Json::Value resp;
    Json::StreamWriterBuilder wbuilder;
    wbuilder["indentation"] = "";

    try {
        if (reader.parse(request, req, false)) {
            HandleJsonRequest(req, resp);
        } else {
            WrapError(Json::nullValue, Errors::ERROR_RPC_JSON_PARSE_ERROR,
                      Errors::GetErrorMessage(Errors::ERROR_RPC_JSON_PARSE_ERROR), resp);
        }
    } catch (const Json::Exception& e) {
        WrapError(Json::nullValue, Errors::ERROR_RPC_JSON_PARSE_ERROR,
                  Errors::GetErrorMessage(Errors::ERROR_RPC_JSON_PARSE_ERROR), resp);
    }

    if (resp != Json::nullValue)
        retValue = Json::writeString(wbuilder, resp);
}

void ServerProtocolHandler::ProcessRequest(const Json::Value& request, Json::Value& response) {
    Procedure& method = procedures[request[KEY_REQUEST_METHODNAME].asString()];
    Json::Value result;

    handler.HandleMethodCall(method, request[KEY_REQUEST_PARAMETERS], result);
    WrapResult(request, response, result);
}

int ServerProtocolHandler::ValidateRequest(const Json::Value& request) {
    int error = 0;
    Procedure proc;
    if (!ValidateRequestFields(request)) {
        error = Errors::ERROR_RPC_INVALID_REQUEST;
    } else {
        map<string, Procedure>::iterator it = procedures.find(request[KEY_REQUEST_METHODNAME].asString());
        if (it != procedures.end()) {
            proc = it->second;
            if (!proc.ValdiateParameters(request[KEY_REQUEST_PARAMETERS])) {
                error = Errors::ERROR_RPC_INVALID_PARAMS;
            }
        } else {
            error = Errors::ERROR_RPC_METHOD_NOT_FOUND;
        }
    }
    return error;
}

const char* KEY_REQUEST_METHODNAME = "method";
const char* KEY_REQUEST_ID = "id";
const char* KEY_REQUEST_PARAMETERS = "params";
const char* KEY_RESPONSE_ERROR = "error";
const char* KEY_RESPONSE_RESULT = "result";
const char* KEY_REQUEST_VERSION = "jsonrpc";
const char* JSON_RPC_VERSION2 = "2.0";
