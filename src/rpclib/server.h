/*-----------------------------------------------------------------------
 * This file was originally part of libjson-rpc-cpp which has been
 * almost completely re-written to remove anything not directly needed
 * by the Ethereum RPC. It retains the original license as described in
 * LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite circa 2020)
 *---------------------------------------------------------------------*/
#pragma once

#include <map>
#include <string>
#include <vector>

#include <rpclib/procedure.h>
#include <rpclib/server_base.h>
#include <rpclib/server_handler.h>
#include <rpclib/http_server.h>

namespace jsonrpc {

    template <class S>
    class Server : public Server_base {
      public:
        typedef void (S::*methodPointer_t)(const Json::Value& parameter, Json::Value& result);

        Server(HttpServer& connector) : connection(connector) {
            handler = new ServerProtocolHandler(*this);
            connector.SetHandler(handler);
        }

        virtual ~Server() {
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
