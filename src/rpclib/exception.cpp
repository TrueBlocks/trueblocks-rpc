/*-----------------------------------------------------------------------
 * This file was originally part of libjson-rpc-cpp which has been
 * almost completely re-written to remove anything not directly needed
 * by the Ethereum RPC. It retains the original license as described in
 * LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite circa 2020)
 *---------------------------------------------------------------------*/
#include <rpclib/exception.h>

using namespace jsonrpc;

JsonRpcException::JsonRpcException(int code) : code(code), message(Errors::GetErrorMessage(code)) {
    setWhatMessage();
}

JsonRpcException::JsonRpcException(int code, const string& m) : code(code), message(Errors::GetErrorMessage(code)) {
    if (message != "")
        message = message + ": ";
    message = message + m;
    setWhatMessage();
}

JsonRpcException::JsonRpcException(int code, const string& m, const Json::Value& data)
    : code(code), message(Errors::GetErrorMessage(code)), data(data) {
    if (message != "")
        message = message + ": ";
    message = message + m;
    setWhatMessage();
}

JsonRpcException::JsonRpcException(const string& m) : code(0), message(m) {
    setWhatMessage();
}

JsonRpcException::~JsonRpcException() throw() {
}

int JsonRpcException::GetCode() const {
    return code;
}

const string& JsonRpcException::GetMessage() const {
    return message;
}

const Json::Value& JsonRpcException::GetData() const {
    return data;
}

const char* JsonRpcException::what() const throw() {
    return whatString.c_str();
}

void JsonRpcException::setWhatMessage() {
    if (code != 0) {
        stringstream ss;
        ss << "Exception " << code << " : " << message;
        if (data != Json::nullValue)
            ss << ", data: " << data.toStyledString();
        whatString = ss.str();
    } else {
        whatString = message;
    }
}
