/*-----------------------------------------------------------------------
 * This file is part of TrueBlocks-RPC, which is a near-complete rewrite
 * of libjson-rpc-cpp removing anything not directly needed to support
 * an Ethereum RPC client and server. It retains the original license
 * as described in LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite 2021)
 *---------------------------------------------------------------------*/
#include <rpclib/batch_request.h>
#include <rpclib/client_handler.h>
#include <rpclib/procedure.h>

using namespace jsonrpc;
using namespace std;

//---------------------------------------------------------------------------------------
BatchRequest::BatchRequest() : id(1) {
}

//---------------------------------------------------------------------------------------
int BatchRequest::addCall(const string& methodname, const jsonval_t& params) {
    jsonval_t call;
    call[KEY_PROTOCOL_VERSION] = "2.0";
    call[KEY_PROCEDURE_NAME] = methodname;

    if (params.isNull() || params.size() > 0)
        call[KEY_PARAMETER] = params;

    call[KEY_ID] = id++;
    result.append(call);

    return call[KEY_ID].asInt();
}

//---------------------------------------------------------------------------------------
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
