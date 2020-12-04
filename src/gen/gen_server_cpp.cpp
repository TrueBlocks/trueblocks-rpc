/*-----------------------------------------------------------------------
 * This file was originally part of libjson-rpc-cpp which has been
 * almost completely re-written to remove anything not directly needed
 * by the Ethereum RPC. It retains the original license as described in
 * LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite circa 2020)
 *---------------------------------------------------------------------*/
#include <algorithm>
#include <sstream>

#include <gen/gen.h>

using namespace std;
using namespace jsonrpc;

//---------------------------------------------------------------------------------------
extern const char* TEMPLATE_CPPSERVER_METHODBINDING;
extern const char* TEMPLATE_CPPSERVER_SIGCLASS;
extern const char* TEMPLATE_CPPSERVER_SIGCONSTRUCTOR;
extern const char* TEMPLATE_CPPSERVER_SIGMETHOD;
extern const char* TEMPLATE_CPPSERVER_SIGMETHOD_WITHOUT_PARAMS;
extern const char* TEMPLATE_SERVER_ABSTRACTDEFINITION;
extern const char* TEMPLATE_CPPSERVER_GUARD;

//---------------------------------------------------------------------------------------
void CppServerCodeGenerator::generateStub() {
    StringVector classname;
    splitPackages(stubname, classname);
    writeLine("/**");
    writeLine(" * This file is generated by jsonrpcstub, DO NOT CHANGE IT MANUALLY!");
    writeLine(" */");
    writeNewLine();

    writeLine(TEMPLATE_CPPSERVER_GUARD);
    writeNewLine();

    writeLine("#include <rpclib/rpclib.h>");
    writeNewLine();

    int depth = namespaceOpen(stubname);

    writeLine(substitute(TEMPLATE_CPPSERVER_SIGCLASS, "<stubname>", classname.at(classname.size() - 1)));
    writeLine("{");
    increaseIndentation();
    writeLine("public:");
    increaseIndentation();

    writeLine(substitute(TEMPLATE_CPPSERVER_SIGCONSTRUCTOR, "<stubname>", classname.at(classname.size() - 1)));
    writeLine("{");
    generateBindings();
    writeLine("}");

    writeNewLine();

    generateProcedureDefinitions();

    generateAbstractDefinitions();

    decreaseIndentation();
    decreaseIndentation();
    writeLine("};");
    writeNewLine();

    namespaceClose(depth);
}

//---------------------------------------------------------------------------------------
void CppServerCodeGenerator::generateBindings() {
    string tmp;
    increaseIndentation();
    for (ProcedureVector::const_iterator it = procedures.begin(); it != procedures.end(); ++it) {
        const Procedure& proc = *it;
        tmp = TEMPLATE_CPPSERVER_METHODBINDING;
        replaceAll(tmp, "<rawprocedurename>", proc.GetProcedureName());
        replaceAll(tmp, "<procedurename>", normalizeString(proc.GetProcedureName()));
        replaceAll(tmp, "<returntype>", toString(proc.GetReturnType()));
        replaceAll(tmp, "<parameterlist>", generateBindingParameterlist(proc));
        replaceAll(tmp, "<stubname>", stubname);

        if (proc.GetParameterDeclarationType() == PARAMS_BY_NAME) {
            replaceAll(tmp, "<paramtype>", "PARAMS_BY_NAME");
        } else {
            replaceAll(tmp, "<paramtype>", "PARAMS_BY_POSITION");
        }

        writeLine(tmp);
    }
    decreaseIndentation();
}

//---------------------------------------------------------------------------------------
void CppServerCodeGenerator::generateProcedureDefinitions() {
    for (ProcedureVector::const_iterator it = procedures.begin(); it != procedures.end(); ++it) {
        const Procedure& proc = *it;
        if (!proc.GetParameters().empty()) {
            writeLine(
                substitute(TEMPLATE_CPPSERVER_SIGMETHOD, "<procedurename>", normalizeString(proc.GetProcedureName())));
        } else {
            writeLine(substitute(TEMPLATE_CPPSERVER_SIGMETHOD_WITHOUT_PARAMS, "<procedurename>",
                                 normalizeString(proc.GetProcedureName())));
        }

        writeLine("{");
        increaseIndentation();

        write("response = ");
        write(normalizeString(proc.GetProcedureName()) + "(");
        generateParameterMapping(proc);
        writeLine(");");

        decreaseIndentation();
        writeLine("}");
    }
}

//---------------------------------------------------------------------------------------
void CppServerCodeGenerator::generateAbstractDefinitions() {
    string tmp;
    for (ProcedureVector::iterator it = procedures.begin(); it != procedures.end(); ++it) {
        Procedure& proc = *it;
        tmp = TEMPLATE_SERVER_ABSTRACTDEFINITION;
        string returntype = "void";
        returntype = toCppType(proc.GetReturnType(), true);
        replaceAll(tmp, "<returntype>", returntype);
        replaceAll(tmp, "<procedurename>", normalizeString(proc.GetProcedureName()));
        replaceAll(tmp, "<parameterlist>", generateParameterDeclarationList(proc));
        writeLine(tmp);
    }
}

//---------------------------------------------------------------------------------------
string CppServerCodeGenerator::generateBindingParameterlist(const Procedure& proc) {
    stringstream parameter;
    const parameterNameList_t& list = proc.GetParameters();

    for (parameterNameList_t::const_iterator it2 = list.begin(); it2 != list.end(); ++it2) {
        parameter << "\"" << it2->first << "\"," << toString(it2->second) << ",";
    }
    return parameter.str();
}

//---------------------------------------------------------------------------------------
extern string toCppConversion(jsontype_t type);

//---------------------------------------------------------------------------------------
void CppServerCodeGenerator::generateParameterMapping(const Procedure& proc) {
    string tmp;
    const parameterNameList_t& params = proc.GetParameters();
    int i = 0;
    for (parameterNameList_t::const_iterator it2 = params.begin(); it2 != params.end(); ++it2) {
        if (proc.GetParameterDeclarationType() == PARAMS_BY_NAME) {
            tmp = "request[\"" + it2->first + "\"]" + toCppConversion(it2->second);
        } else {
            stringstream tmp2;
            tmp2 << "request[" << i << "u]" << toCppConversion(it2->second);
            tmp = tmp2.str();
        }
        write(tmp);
        if (it2 != --params.end()) {
            write(", ");
        }
        i++;
    }
}

//---------------------------------------------------------------------------------------
const char* TEMPLATE_CPPSERVER_METHODBINDING =
    "bindAndAddMethod(jsonrpc::Procedure(\"<rawprocedurename>\", <paramtype>, <returntype>, <parameterlist> "
    "NULL), &<stubname>::<procedurename>I);";
const char* TEMPLATE_CPPSERVER_SIGCLASS = "class <stubname> : public jsonrpc::Server<<stubname>>";
const char* TEMPLATE_CPPSERVER_SIGCONSTRUCTOR =
    "<stubname>(jsonrpc::HttpServer &conn) : jsonrpc::Server<<stubname>>(conn)";
const char* TEMPLATE_CPPSERVER_SIGMETHOD =
    "inline virtual void <procedurename>I(const Json::Value &request, Json::Value &response)";
const char* TEMPLATE_CPPSERVER_SIGMETHOD_WITHOUT_PARAMS =
    "inline virtual void <procedurename>I(const Json::Value &/*request*/, Json::Value &response)";
const char* TEMPLATE_SERVER_ABSTRACTDEFINITION = "virtual <returntype> <procedurename>(<parameterlist>) = 0;";
const char* TEMPLATE_CPPSERVER_GUARD = "#pragma once";
