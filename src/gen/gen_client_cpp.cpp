/*-----------------------------------------------------------------------
 * This file is part of TrueBlocks-RPC, which is a near-complete rewrite
 * of libjson-rpc-cpp removing anything not directly needed to support
 * an Ethereum RPC client and server. It retains the original license
 * as described in LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite 2021)
 *---------------------------------------------------------------------*/
#include <algorithm>

#include <gen/gen.h>

using namespace std;
using namespace jsonrpc;

//---------------------------------------------------------------------------------------
extern const char* TEMPLATE_CPPCLIENT_SIGCLASS;
extern const char* TEMPLATE_CPPCLIENT_SIGCONSTRUCTOR;
extern const char* TEMPLATE_CPPCLIENT_SIGMETHOD;
extern const char* TEMPLATE_CPPCLIENT_SIGCOMMENT;
extern const char* TEMPLATE_NAMED_ASSIGNMENT;
extern const char* TEMPLATE_POSITION_ASSIGNMENT;
extern const char* TEMPLATE_METHODCALL;
extern const char* TEMPLATE_RETURNCHECK;
extern const char* TEMPLATE_RETURN;
extern const char* TEMPLATE_CPPSERVER_GUARD;

//---------------------------------------------------------------------------------------
void CppClientCodeGenerator::generateStub() {
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

    writeLine(substitute(TEMPLATE_CPPCLIENT_SIGCLASS, "<stubname>", classname.at(classname.size() - 1)));
    writeLine("{");
    increaseIndentation();
    writeLine("public:");
    increaseIndentation();

    writeLine(substitute(TEMPLATE_CPPCLIENT_SIGCONSTRUCTOR, "<stubname>", classname.at(classname.size() - 1)));
    writeNewLine();

    for (unsigned int i = 0; i < procedures.size(); i++) {
        generateMethod(procedures[i]);
    }

    decreaseIndentation();
    decreaseIndentation();
    writeLine("};");
    writeNewLine();

    namespaceClose(depth);
}

//---------------------------------------------------------------------------------------
void CppClientCodeGenerator::generateMethod(Procedure& proc) {
    string procsignature = TEMPLATE_CPPCLIENT_SIGMETHOD;
    string proccomment = TEMPLATE_CPPCLIENT_SIGCOMMENT;
    string returntype = toCppType(proc.GetReturnType(), true);

    replaceAll(procsignature, "<returntype>", returntype);
    replaceAll(procsignature, "<methodname>", normalizeString(proc.GetProcedureName()));
    replaceAll(procsignature, "<parameters>", generateParameterDeclarationList(proc));

    replaceAll(proccomment, "<returntype>", returntype);
    replaceAll(proccomment, "<methodname>", normalizeString(proc.GetProcedureName()));
    replaceAll(proccomment, "<parameters>", generateParameterDeclarationList(proc));

    writeLine(proccomment);
    writeLine(procsignature);
    writeLine("{");
    increaseIndentation();

    writeLine("jsonval_t p;");

    generateAssignments(proc);
    generateProcCall(proc);

    decreaseIndentation();
    writeLine("}");
}

//---------------------------------------------------------------------------------------
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
            writeLine(assignment);
        }
    } else {
        writeLine("p = Json::nullValue;");
    }
}

//---------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------
void CppClientCodeGenerator::generateProcCall(Procedure& proc) {
    string call;
    call = TEMPLATE_METHODCALL;
    writeLine(substitute(call, "<name>", proc.GetProcedureName()));
    call = TEMPLATE_RETURNCHECK;
    replaceAll(call, "<cast>", isCppConversion(proc.GetReturnType()));
    writeLine(call);
    increaseIndentation();
    call = TEMPLATE_RETURN;
    replaceAll(call, "<cast>", toCppConversion(proc.GetReturnType()));
    writeLine(call);
    decreaseIndentation();
    writeLine("else");
    increaseIndentation();
    writeLine("throw jsonrpc::JsonRpcException(jsonrpc::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());");
    decreaseIndentation();
}

//---------------------------------------------------------------------------------------
const char* TEMPLATE_CPPCLIENT_SIGCLASS = "class <stubname> : public jsonrpc::Client";
const char* TEMPLATE_CPPCLIENT_SIGCONSTRUCTOR = "<stubname>(jsonrpc::HttpClient &conn) : jsonrpc::Client(conn) {}";
const char* TEMPLATE_CPPCLIENT_SIGMETHOD = "<returntype> <methodname>(<parameters>) ";
const char* TEMPLATE_CPPCLIENT_SIGCOMMENT = "// Implements client side of <methodname>";
const char* TEMPLATE_NAMED_ASSIGNMENT = "p[\"<paramname>\"] = <paramname>;";
const char* TEMPLATE_POSITION_ASSIGNMENT = "p.append(<paramname>);";
const char* TEMPLATE_METHODCALL = "jsonval_t result = CallMethod(\"<name>\",p);";
const char* TEMPLATE_RETURNCHECK = "if (result<cast>)";
const char* TEMPLATE_RETURN = "return result<cast>;";
