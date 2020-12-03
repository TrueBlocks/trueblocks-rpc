/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    exception.h
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef JSONRPC_CPP_EXCEPTION_H_
#define JSONRPC_CPP_EXCEPTION_H_

#include <string>
#include <sstream>
#include <exception>

#include "errors.h"

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
#endif /* JSONRPC_CPP_EXCEPTION_H_ */
