/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    client.cpp
 * @date    03.01.2013
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include <rpclib/client.h>
#include <rpclib/clientprotocolhandler.h>

using namespace jsonrpc;

Client::Client(HttpClient& connector, bool omitEndingLineFeed) : connector(connector) {
    protocol = new ClientProtocolHandler(omitEndingLineFeed);
}

Client::~Client() {
    delete protocol;
}

void Client::CallMethod(const string& name, const Json::Value& parameter, Json::Value& result) {
    string request, response;
    protocol->BuildRequest(name, parameter, request, false);
    connector.SendRPCMessage(request, response);
    protocol->HandleResponse(response, result);
}

void Client::CallProcedures(const BatchCall& calls, BatchResponse& result) {
    string request, response;
    request = calls.toString();
    connector.SendRPCMessage(request, response);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    Json::Reader reader;
#pragma clang diagnostic pop
    Json::Value tmpresult;

    try {
        if (!reader.parse(response, tmpresult) || !tmpresult.isArray()) {
            throw JsonRpcException(Errors::ERROR_CLIENT_INVALID_RESPONSE, "Array expected.");
        }
    } catch (const Json::Exception& e) {
        throw JsonRpcException(Errors::ERROR_RPC_JSON_PARSE_ERROR,
                               Errors::GetErrorMessage(Errors::ERROR_RPC_JSON_PARSE_ERROR), response);
    }

    for (unsigned int i = 0; i < tmpresult.size(); i++) {
        if (tmpresult[i].isObject()) {
            Json::Value singleResult;
            try {
                Json::Value id = protocol->HandleResponse(tmpresult[i], singleResult);
                result.addResponse(id, singleResult, false);
            } catch (JsonRpcException& ex) {
                Json::Value id = -1;
                if (tmpresult[i].isMember("id"))
                    id = tmpresult[i]["id"];
                result.addResponse(id, tmpresult[i]["error"], true);
            }
        } else
            throw JsonRpcException(Errors::ERROR_CLIENT_INVALID_RESPONSE, "Object in Array expected.");
    }
}

BatchResponse Client::CallProcedures(const BatchCall& calls) {
    BatchResponse result;
    CallProcedures(calls, result);
    return result;
}

Json::Value Client::CallMethod(const string& name, const Json::Value& parameter) {
    Json::Value result;
    CallMethod(name, parameter, result);
    return result;
}

void Client::CallNotification(const string& name, const Json::Value& parameter) {
    string request, response;
    protocol->BuildRequest(name, parameter, request, true);
    connector.SendRPCMessage(request, response);
}
