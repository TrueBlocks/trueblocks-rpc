/*-----------------------------------------------------------------------
 * This file is part of TrueBlocks-RPC, which is a near-complete rewrite
 * of libjson-rpc-cpp removing anything not directly needed to support
 * an Ethereum RPC client and server. It retains the original license
 * as described in LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite 2021)
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

    //---------------------------------------------------------------------------------------
    template <class S>
    class Server : public Server_base {
      public:
        typedef void (S::*methodPointer_t)(const jsonval_t& parameter, jsonval_t& result);

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

        virtual void HandleMethodCall(Procedure& proc, const jsonval_t& input, jsonval_t& output) {
            S* instance = dynamic_cast<S*>(this);
            (instance->*methods[proc.GetName()])(input, output);
        }

      protected:
        bool bindAndAddMethod(const Procedure& proc, methodPointer_t pointer) {
            if (!symbolExists(proc.GetName())) {
                handler->AddProcedure(proc);
                methods[proc.GetName()] = pointer;
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
