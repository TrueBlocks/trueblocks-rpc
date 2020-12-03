/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    CppClientCodeGenerator.cpp
 * @date    01.05.2013
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "generate.h"

extern const char* TEMPLATE_CPPCLIENT_SIGCLASS;
extern const char* TEMPLATE_CPPCLIENT_SIGCONSTRUCTOR;
extern const char* TEMPLATE_CPPCLIENT_SIGMETHOD;
extern const char* TEMPLATE_CPPCLIENT_SIGCOMMENT;
extern const char* TEMPLATE_NAMED_ASSIGNMENT;
extern const char* TEMPLATE_POSITION_ASSIGNMENT;
extern const char* TEMPLATE_METHODCALL;
extern const char* TEMPLATE_NOTIFICATIONCALL;
extern const char* TEMPLATE_RETURNCHECK;
extern const char* TEMPLATE_RETURN;
extern const char* TEMPLATE_CPPSERVER_GUARD1;
extern const char* TEMPLATE_CPPSERVER_GUARD2;
extern const char* TEMPLATE_EPILOG;

using namespace std;
using namespace jsonrpc;

void CppClientCodeGenerator::generateStub() {
    StringVector classname;
    splitPackages(stubname, classname);
    writeLine("/**");
    writeLine(" * This file is generated by jsonrpcstub, DO NOT CHANGE IT MANUALLY!");
    writeLine(" */");
    writeNewLine();

    string stub_upper = stubname;
    std::transform(stub_upper.begin(), stub_upper.end(), stub_upper.begin(), ::toupper);
    replaceAll(stub_upper, "::", "_");

    writeLine(substitute(TEMPLATE_CPPSERVER_GUARD1, "<STUBNAME>", stub_upper));
    writeLine(substitute(TEMPLATE_CPPSERVER_GUARD2, "<STUBNAME>", stub_upper));

    writeNewLine();
    this->writeLine("#include <jsonrpccpp/rpclib.h>");
    this->writeNewLine();

    int depth = namespaceOpen(stubname);

    this->writeLine(substitute(TEMPLATE_CPPCLIENT_SIGCLASS, "<stubname>", classname.at(classname.size() - 1)));
    this->writeLine("{");
    this->increaseIndentation();
    this->writeLine("public:");
    this->increaseIndentation();

    this->writeLine(substitute(TEMPLATE_CPPCLIENT_SIGCONSTRUCTOR, "<stubname>", classname.at(classname.size() - 1)));
    this->writeNewLine();

    for (unsigned int i = 0; i < procedures.size(); i++) {
        this->generateMethod(procedures[i]);
    }

    this->decreaseIndentation();
    this->decreaseIndentation();
    this->writeLine("};");
    this->writeNewLine();

    namespaceClose(depth);
    stub_upper = this->stubname;
    std::transform(stub_upper.begin(), stub_upper.end(), stub_upper.begin(), ::toupper);
    replaceAll(stub_upper, "::", "_");
    writeLine(substitute(TEMPLATE_EPILOG, "<STUBNAME>", stub_upper));
}

void CppClientCodeGenerator::generateMethod(Procedure& proc) {
    string procsignature = TEMPLATE_CPPCLIENT_SIGMETHOD;
    string proccomment = TEMPLATE_CPPCLIENT_SIGCOMMENT;
    string returntype = toCppType(proc.GetReturnType(), true);
    if (proc.GetProcedureType() == RPC_NOTIFICATION)
        returntype = "void";

    replaceAll(procsignature, "<returntype>", returntype);
    replaceAll(procsignature, "<methodname>", normalizeString(proc.GetProcedureName()));
    replaceAll(procsignature, "<parameters>", generateParameterDeclarationList(proc));

    replaceAll(proccomment, "<returntype>", returntype);
    replaceAll(proccomment, "<methodname>", normalizeString(proc.GetProcedureName()));
    replaceAll(proccomment, "<parameters>", generateParameterDeclarationList(proc));

    this->writeLine(proccomment);
    this->writeLine(procsignature);
    this->writeLine("{");
    this->increaseIndentation();

    this->writeLine("Json::Value p;");

    generateAssignments(proc);
    generateProcCall(proc);

    this->decreaseIndentation();
    this->writeLine("}");
}

void CppClientCodeGenerator::generateAssignments(Procedure& proc) {
    string assignment;
    parameterNameList_t list = proc.GetParameters();
    if (list.size() > 0) {
        for (parameterNameList_t::iterator it = list.begin(); it != list.end(); ++it) {
            if (proc.GetParameterDeclarationType() == PARAMS_BY_NAME) {
                assignment = TEMPLATE_NAMED_ASSIGNMENT;
            } else {
                assignment = TEMPLATE_POSITION_ASSIGNMENT;
            }
            replaceAll(assignment, "<paramname>", it->first);
            this->writeLine(assignment);
        }
    } else {
        this->writeLine("p = Json::nullValue;");
    }
}

string isCppConversion(jsontype_t type) {
    string result;
    switch (type) {
        case JSON_BOOLEAN:
            result = ".isBool()";
            break;
        case JSON_INTEGER:
            result = ".isIntegral()";
            break;
        case JSON_REAL:
            result = ".isDouble()";
            break;
        case JSON_NUMERIC:
            result = ".isNumeric()";
            break;
        case JSON_STRING:
            result = ".isString()";
            break;
        case JSON_OBJECT:
            result = ".isObject()";
            break;
        case JSON_ARRAY:
            result = ".isArray()";
            break;
    }
    return result;
}

string toCppConversion(jsontype_t type) {
    string result;
    switch (type) {
        case JSON_BOOLEAN:
            result = ".asBool()";
            break;
        case JSON_INTEGER:
            result = ".asInt()";
            break;
        case JSON_REAL:
            result = ".asDouble()";
            break;
        case JSON_NUMERIC:
            result = ".asDouble()";
            break;
        case JSON_STRING:
            result = ".asString()";
            break;
        default:
            result = "";
            break;
    }
    return result;
}

void CppClientCodeGenerator::generateProcCall(Procedure& proc) {
    string call;
    if (proc.GetProcedureType() == RPC_METHOD) {
        call = TEMPLATE_METHODCALL;
        this->writeLine(substitute(call, "<name>", proc.GetProcedureName()));
        call = TEMPLATE_RETURNCHECK;
        replaceAll(call, "<cast>", isCppConversion(proc.GetReturnType()));
        this->writeLine(call);
        this->increaseIndentation();
        call = TEMPLATE_RETURN;
        replaceAll(call, "<cast>", toCppConversion(proc.GetReturnType()));
        this->writeLine(call);
        this->decreaseIndentation();
        this->writeLine("else");
        this->increaseIndentation();
        this->writeLine(
            "throw "
            "jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_"
            "INVALID_RESPONSE, result.toStyledString());");
        this->decreaseIndentation();
    } else {
        call = TEMPLATE_NOTIFICATIONCALL;
        replaceAll(call, "<name>", proc.GetProcedureName());
        this->writeLine(call);
    }
}

const char* TEMPLATE_CPPCLIENT_SIGCLASS = "class <stubname> : public jsonrpc::Client";
const char* TEMPLATE_CPPCLIENT_SIGCONSTRUCTOR = "<stubname>(jsonrpc::HttpClient &conn) : jsonrpc::Client(conn) {}";
const char* TEMPLATE_CPPCLIENT_SIGMETHOD = "<returntype> <methodname>(<parameters>) ";
const char* TEMPLATE_CPPCLIENT_SIGCOMMENT = "// Implements client side of <methodname>";
const char* TEMPLATE_NAMED_ASSIGNMENT = "p[\"<paramname>\"] = <paramname>;";
const char* TEMPLATE_POSITION_ASSIGNMENT = "p.append(<paramname>);";
const char* TEMPLATE_METHODCALL = "Json::Value result = this->CallMethod(\"<name>\",p);";
const char* TEMPLATE_NOTIFICATIONCALL = "this->CallNotification(\"<name>\",p);";
const char* TEMPLATE_RETURNCHECK = "if (result<cast>)";
const char* TEMPLATE_RETURN = "return result<cast>;";