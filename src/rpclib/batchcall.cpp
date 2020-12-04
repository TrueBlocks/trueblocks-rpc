/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    batchcall.cpp
 * @date    15.10.2013
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include <rpclib/batchcall.h>
#include <rpclib/clientprotocolhandler.h>
#include <rpclib/procedure.h>

using namespace jsonrpc;
using namespace std;

BatchCall::BatchCall() : id(1) {
}

int BatchCall::addCall(const string& methodname, const Json::Value& params, bool isNotification) {
    Json::Value call;
    call[ClientProtocolHandler::KEY_PROTOCOL_VERSION] = "2.0";
    call[ClientProtocolHandler::KEY_PROCEDURE_NAME] = methodname;

    if (params.isNull() || params.size() > 0)
        call[ClientProtocolHandler::KEY_PARAMETER] = params;

    if (!isNotification) {
        call[ClientProtocolHandler::KEY_ID] = id++;
    }
    result.append(call);

    if (isNotification)
        return -1;
    return call[ClientProtocolHandler::KEY_ID].asInt();
}

string BatchCall::toString(bool fast) const {
    string res;
    if (fast) {
        Json::StreamWriterBuilder wbuilder;
        wbuilder["indentation"] = "";
        res = Json::writeString(wbuilder, result);
    } else {
        Json::StreamWriterBuilder wbuilder;
        res = Json::writeString(wbuilder, result);
    }
    return res;
}
