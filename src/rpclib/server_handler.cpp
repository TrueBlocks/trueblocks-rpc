/*-----------------------------------------------------------------------
 * This file is part of TrueBlocks-RPC, which is a near-complete rewrite
 * of libjson-rpc-cpp removing anything not directly needed to support
 * an Ethereum RPC client and server. It retains the original license
 * as described in LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite 2021)
 *---------------------------------------------------------------------*/
#include <iostream>

#include <rpclib/server_handler.h>
#include <rpclib/errors.h>

using namespace std;
using namespace jsonrpc;

//---------------------------------------------------------------------------------------
ServerProtocolHandler::ServerProtocolHandler(Server_base& h) : handler(h) {
}

//---------------------------------------------------------------------------------------
ServerProtocolHandler& ServerProtocolHandler::operator=(const ServerProtocolHandler&) {
    return *this;
}

//---------------------------------------------------------------------------------------
void ServerProtocolHandler::HandleJsonRequest(const jsonval_t& req, jsonval_t& response) {
    // It could be a Batch Request
    if (req.isArray()) {
        HandleBatchRequest(req, response);
    }  // It could be a simple Request
    else if (req.isObject()) {
        HandleSingleRequest(req, response);
    } else {
        WrapError(Json::nullValue, ERROR_RPC_INVALID_REQUEST, Errors::GetErrorMessage(ERROR_RPC_INVALID_REQUEST),
                  response);
    }
}

//---------------------------------------------------------------------------------------
void ServerProtocolHandler::HandleSingleRequest(const jsonval_t& req, jsonval_t& response) {
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

//---------------------------------------------------------------------------------------
void ServerProtocolHandler::HandleBatchRequest(const jsonval_t& req, jsonval_t& response) {
    if (req.size() == 0)
        WrapError(Json::nullValue, ERROR_RPC_INVALID_REQUEST, Errors::GetErrorMessage(ERROR_RPC_INVALID_REQUEST),
                  response);
    else {
        for (unsigned int i = 0; i < req.size(); i++) {
            jsonval_t result;
            HandleSingleRequest(req[i], result);
            if (result != Json::nullValue)
                response.append(result);
        }
    }
}

//---------------------------------------------------------------------------------------
extern const char* KEY_REQUEST_METHODNAME;
extern const char* KEY_REQUEST_ID;
extern const char* KEY_REQUEST_PARAMETERS;
extern const char* KEY_RESPONSE_ERROR;
extern const char* KEY_RESPONSE_RESULT;
extern const char* KEY_REQUEST_VERSION;
extern const char* JSON_RPC_VERSION2;

//---------------------------------------------------------------------------------------
bool ServerProtocolHandler::ValidateRequestFields(const jsonval_t& request) {
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

//---------------------------------------------------------------------------------------
void ServerProtocolHandler::WrapResult(const jsonval_t& request, jsonval_t& response, jsonval_t& result) {
    response[KEY_REQUEST_VERSION] = JSON_RPC_VERSION2;
    response[KEY_RESPONSE_RESULT] = result;
    response[KEY_REQUEST_ID] = request[KEY_REQUEST_ID];
}

//---------------------------------------------------------------------------------------
void ServerProtocolHandler::WrapError(const jsonval_t& request, int code, const string& message, jsonval_t& result) {
    result["jsonrpc"] = "2.0";
    if (code == TG_ERROR_SERVER_DEPRECATED || code == TG_ERROR_SERVER_NOTIMPLEMENTED)
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

//---------------------------------------------------------------------------------------
void ServerProtocolHandler::WrapException(const jsonval_t& request, const JsonRpcException& exception,
                                          jsonval_t& result) {
    WrapError(request, exception.GetCode(), exception.GetMessage(), result);
    result["error"]["data"] = exception.GetData();
}

//---------------------------------------------------------------------------------------
void ServerProtocolHandler::AddProcedure(const Procedure& procedure) {
    procedures[procedure.GetName()] = procedure;
}

//---------------------------------------------------------------------------------------
void ServerProtocolHandler::HandleRequest(const string& request, string& retValue) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    Json::Reader reader;
#pragma clang diagnostic pop
    jsonval_t req;
    jsonval_t resp;
    Json::StreamWriterBuilder wbuilder;
    wbuilder["indLevel"] = "";

    try {
        if (reader.parse(request, req, false)) {
            HandleJsonRequest(req, resp);
        } else {
            WrapError(Json::nullValue, ERROR_RPC_JSON_PARSE_ERROR, Errors::GetErrorMessage(ERROR_RPC_JSON_PARSE_ERROR),
                      resp);
        }
    } catch (const Json::Exception& e) {
        WrapError(Json::nullValue, ERROR_RPC_JSON_PARSE_ERROR, Errors::GetErrorMessage(ERROR_RPC_JSON_PARSE_ERROR),
                  resp);
    }

    if (resp != Json::nullValue)
        retValue = Json::writeString(wbuilder, resp);
}

//---------------------------------------------------------------------------------------
void ServerProtocolHandler::ProcessRequest(const jsonval_t& request, jsonval_t& response) {
    Procedure& method = procedures[request[KEY_REQUEST_METHODNAME].asString()];
    jsonval_t result;

    handler.HandleMethodCall(method, request[KEY_REQUEST_PARAMETERS], result);
    WrapResult(request, response, result);
}

//---------------------------------------------------------------------------------------
int ServerProtocolHandler::ValidateRequest(const jsonval_t& request) {
    int error = 0;
    Procedure proc;
    if (!ValidateRequestFields(request)) {
        error = ERROR_RPC_INVALID_REQUEST;
    } else {
        map<string, Procedure>::iterator it = procedures.find(request[KEY_REQUEST_METHODNAME].asString());
        if (it != procedures.end()) {
            proc = it->second;
            if (!proc.ValdiateParameters(request[KEY_REQUEST_PARAMETERS])) {
                error = ERROR_RPC_INVALID_PARAMS;
            }
        } else {
            error = ERROR_RPC_METHOD_NOT_FOUND;
        }
    }
    return error;
}

//---------------------------------------------------------------------------------------
const char* KEY_REQUEST_METHODNAME = "method";
const char* KEY_REQUEST_ID = "id";
const char* KEY_REQUEST_PARAMETERS = "params";
const char* KEY_RESPONSE_ERROR = "error";
const char* KEY_RESPONSE_RESULT = "result";
const char* KEY_REQUEST_VERSION = "jsonrpc";
const char* JSON_RPC_VERSION2 = "2.0";
