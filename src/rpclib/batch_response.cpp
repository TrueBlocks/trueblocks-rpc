/*-----------------------------------------------------------------------
 * This file is part of TrueBlocks-RPC, which is a near-complete rewrite
 * of libjson-rpc-cpp removing anything not directly needed to support
 * an Ethereum RPC client and server. It retains the original license
 * as described in LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite 2021)
 *---------------------------------------------------------------------*/
#include <algorithm>

#include <rpclib/batch_response.h>

using namespace jsonrpc;
using namespace std;

//---------------------------------------------------------------------------------------
BatchResponse::BatchResponse() {
}

//---------------------------------------------------------------------------------------
void BatchResponse::addResponse(const jsonval_t& id, const jsonval_t& response, bool isError) {
    if (isError) {
        errorResponses.push_back(id);
    }
    responses[id] = response;
}

//---------------------------------------------------------------------------------------
jsonval_t BatchResponse::getResult(int id) {
    jsonval_t result;
    jsonval_t i = id;
    getResult(i, result);
    return result;
}

//---------------------------------------------------------------------------------------
void BatchResponse::getResult(const jsonval_t& id, jsonval_t& result) {
    if (getErrorCode(id) == 0)
        result = responses[id];
    else
        result = Json::nullValue;
}

//---------------------------------------------------------------------------------------
int BatchResponse::getErrorCode(const jsonval_t& id) {
    if (std::find(errorResponses.begin(), errorResponses.end(), id) != errorResponses.end()) {
        return responses[id]["code"].asInt();
    }
    return 0;
}

//---------------------------------------------------------------------------------------
string BatchResponse::getErrorMessage(const jsonval_t& id) {
    if (std::find(errorResponses.begin(), errorResponses.end(), id) != errorResponses.end()) {
        return responses[id]["message"].asString();
    }
    return "";
}

//---------------------------------------------------------------------------------------
string BatchResponse::getErrorMessage(int id) {
    jsonval_t i = id;
    return getErrorMessage(i);
}

//---------------------------------------------------------------------------------------
bool BatchResponse::hasErrors() {
    return !errorResponses.empty();
}
