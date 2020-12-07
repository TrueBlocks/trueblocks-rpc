/*-----------------------------------------------------------------------
 * This file is part of TrueBlocks-RPC, which is a near-complete rewrite
 * of libjson-rpc-cpp removing anything not directly needed to support
 * an Ethereum RPC client and server. It retains the original license
 * as described in LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite 2021)
 *---------------------------------------------------------------------*/
#include <algorithm>
#include <sstream>

#include <gen/gen.h>

using namespace std;
using namespace jsonrpc;

//---------------------------------------------------------------------------------------
extern string toCppConversion(jsontype_t type);

//---------------------------------------------------------------------------------------
void CppServerCodeGenerator::generateStub() {
    StringVector parts;
    splitPackages(stubname, parts);
    string className = parts.at(parts.size() - 1);

    extern const char* TEMPLATE_CPP_FILEPROLOG;
    *output << TEMPLATE_CPP_FILEPROLOG << endl;

    int depth = namespaceOpen(stubname);

    *output << "class " << className << " : public jsonrpc::Server<" << className << "> {" << endl;
    *output << "public:" << endl;
    *output << indentIn() << className << "(jsonrpc::HttpServer &conn) : jsonrpc::Server<" << className << ">(conn) {"
            << endl;

    (void)indentIn();
    for (auto proc : procedures) {
        *output << indent() << "bindAndAddMethod(jsonrpc::Procedure(\"" << proc.GetName() << "\", ";
        *output << (proc.GetParameterDecType() == PARAMS_BY_NAME ? "PARAMS_BY_NAME" : "PARAMS_BY_POSITION") << ", ";
        *output << toString(proc.GetType()) << ", ";
        stringstream parameter;
        for (auto param : proc.GetParameters())
            parameter << "\"" << param.first << "\"," << toString(param.second) << ",";
        *output << parameter.str() << " NULL), ";
        *output << "&" << stubname << "::" << normalize(proc.GetName()) << "I);" << endl;
    }
    *output << indentOut() << "}" << endl << endl;

    for (auto proc : procedures) {
        *output << indent() << "inline virtual void " << normalize(proc.GetName()) << "I(const jsonval_t&";
        *output << (proc.nParams() ? "request" : "/*request*/") << ", jsonval_t &response) {" << endl;
        *output << indentIn() << "response = " << normalize(proc.GetName()) << "(";
        int i = 0;
        for (auto param : proc.GetParameters()) {
            if (i != 0)
                *output << ", ";
            if (proc.GetParameterDecType() == PARAMS_BY_NAME) {
                *output << "request[\"" << param.first << "\"]" << toCppConversion(param.second);
            } else {
                *output << "request[" << (i++) << "u]" << toCppConversion(param.second);
            }
        }
        *output << ");" << endl;
        *output << indentOut() << "}" << endl;
    }

    for (auto proc : procedures) {
        *output << indent();
        *output << "virtual " << toCppReturnType(proc.GetType()) << " " << normalize(proc.GetName()) << "(";
        bool first = true;
        for (auto param : proc.GetParameters()) {
            if (!first)
                *output << ", ";
            *output << toCppParamType(param.second) << " " << param.first;
            first = false;
        }
        *output << ") = 0;" << endl;
    }

    *output << indentOut() << "};" << endl;
    namespaceClose(depth);
}
