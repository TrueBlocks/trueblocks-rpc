/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    abstractserver.h
 * @date    30.12.2012
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef JSONRPC_CPP_ABSTRACTSERVER_H_
#define JSONRPC_CPP_ABSTRACTSERVER_H_

#include <rpct/procedure.h>

#include <map>
#include <string>
#include <vector>

#include "iprocedureinvokationhandler.h"
#include "serverprotocolhandler.h"
#include "httpserver.h"

namespace jsonrpc {

    template <class S>
    class AbstractServer : public IProcedureInvokationHandler {
      public:
        typedef void (S::*methodPointer_t)(const Json::Value& parameter, Json::Value& result);

        AbstractServer(HttpServer& connector) : connection(connector) {
            handler = new ServerProtocolHandler(*this);
            connector.SetHandler(handler);
        }

        virtual ~AbstractServer() {
            delete handler;
        }

        bool StartListening() {
            return connection.StartListening();
        }

        bool StopListening() {
            return connection.StopListening();
        }

        virtual void HandleMethodCall(Procedure& proc, const Json::Value& input, Json::Value& output) {
            S* instance = dynamic_cast<S*>(this);
            (instance->*methods[proc.GetProcedureName()])(input, output);
        }

      protected:
        bool bindAndAddMethod(const Procedure& proc, methodPointer_t pointer) {
            if (!symbolExists(proc.GetProcedureName())) {
                handler->AddProcedure(proc);
                methods[proc.GetProcedureName()] = pointer;
                return true;
            }
            return false;
        }

      private:
        HttpServer& connection;
        ServerProtocolHandler* handler;
        std::map<string, methodPointer_t> methods;

        bool symbolExists(const string& name) {
            if (methods.find(name) != methods.end())
                return true;
            return false;
        }
    };

} /* namespace jsonrpc */
#endif /* JSONRPC_CPP_ABSTRACTSERVER_H_ */
