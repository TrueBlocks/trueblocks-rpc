/*-----------------------------------------------------------------------
 * This file was originally part of libjson-rpc-cpp which has been
 * almost completely re-written to remove anything not directly needed
 * by the Ethereum RPC. It retains the original license as described in
 * LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite circa 2020)
 *---------------------------------------------------------------------*/
#include <rpclib/client.h>
#include <rpclib/client_handler.h>

using namespace jsonrpc;

//---------------------------------------------------------------------------------------
Client::Client(HttpClient& connector) : connector(connector) {
    protocol = new ClientProtocolHandler();
}

//---------------------------------------------------------------------------------------
Client::~Client() {
    delete protocol;
}

//---------------------------------------------------------------------------------------
void Client::CallProcedures(const BatchRequest& calls, BatchResponse& result) {
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
            throw JsonRpcException(ERROR_CLIENT_INVALID_RESPONSE, "Array expected.");
        }
    } catch (const Json::Exception& e) {
        throw JsonRpcException(ERROR_RPC_JSON_PARSE_ERROR, Errors::GetErrorMessage(ERROR_RPC_JSON_PARSE_ERROR),
                               response);
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
            throw JsonRpcException(ERROR_CLIENT_INVALID_RESPONSE, "Object in Array expected.");
    }
}

//---------------------------------------------------------------------------------------
BatchResponse Client::CallProcedures(const BatchRequest& calls) {
    BatchResponse result;
    CallProcedures(calls, result);
    return result;
}

//---------------------------------------------------------------------------------------
void Client::CallMethod(const string& name, const Json::Value& parameter, Json::Value& result) {
    string request, response;
    protocol->BuildRequest(name, parameter, request);
    connector.SendRPCMessage(request, response);
    protocol->HandleResponse(response, result);
}

//---------------------------------------------------------------------------------------
Json::Value Client::CallMethod(const string& name, const Json::Value& parameter) {
    Json::Value result;
    CallMethod(name, parameter, result);
    return result;
}
