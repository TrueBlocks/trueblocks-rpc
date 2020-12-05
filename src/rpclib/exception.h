/*-----------------------------------------------------------------------
 * This file is part of TrueBlocks-RPC, which is a near-complete rewrite
 * of libjson-rpc-cpp removing anything not directly needed to support
 * an Ethereum RPC client and server. It retains the original license
 * as described in LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite 2021)
 *---------------------------------------------------------------------*/
#pragma once

#include <string>
#include <sstream>
#include <exception>

#include <rpclib/errors.h>

namespace jsonrpc {
    //---------------------------------------------------------------------------------------
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
