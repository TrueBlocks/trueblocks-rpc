/*-----------------------------------------------------------------------
 * This file was originally part of libjson-rpc-cpp which has been
 * almost completely re-written to remove anything not directly needed
 * by the Ethereum RPC. It retains the original license as described in
 * LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite circa 2020)
 *---------------------------------------------------------------------*/
#pragma once

#include <string>
#include <sstream>
#include <exception>

#include <rpclib/errors.h>

namespace jsonrpc {
    class JsonRpcException : public std::exception {
      public:
        JsonRpcException(int code);
        JsonRpcException(int code, const string& message);
        JsonRpcException(int code, const string& message, const Json::Value& data);
        JsonRpcException(const string& message);

        virtual ~JsonRpcException() throw();

        int GetCode() const;
        const string& GetMessage() const;
        const Json::Value& GetData() const;

        virtual const char* what() const throw();

      private:
        int code;
        string message;
        string whatString;
        Json::Value data;
        void setWhatMessage();
    };

} /* namespace jsonrpc */
