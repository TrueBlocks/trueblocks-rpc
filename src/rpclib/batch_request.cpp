/*-----------------------------------------------------------------------
 * This file was originally part of libjson-rpc-cpp which has been
 * almost completely re-written to remove anything not directly needed
 * by the Ethereum RPC. It retains the original license as described in
 * LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite circa 2020)
 *---------------------------------------------------------------------*/
#include <rpclib/batch_request.h>
#include <rpclib/client_handler.h>
#include <rpclib/procedure.h>

using namespace jsonrpc;
using namespace std;

BatchRequest::BatchRequest() : id(1) {
}

int BatchRequest::addCall(const string& methodname, const Json::Value& params) {
    Json::Value call;
    call[ClientProtocolHandler::KEY_PROTOCOL_VERSION] = "2.0";
    call[ClientProtocolHandler::KEY_PROCEDURE_NAME] = methodname;

    if (params.isNull() || params.size() > 0)
        call[ClientProtocolHandler::KEY_PARAMETER] = params;

    call[ClientProtocolHandler::KEY_ID] = id++;
    result.append(call);

    return call[ClientProtocolHandler::KEY_ID].asInt();
}

string BatchRequest::toString(bool fast) const {
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
