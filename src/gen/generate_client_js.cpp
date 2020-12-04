/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    generate_client_js.cpp
 * @date    10/22/2014
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "generate.h"
#include <algorithm>

using namespace jsonrpc;
using namespace std;

#define TEMPLATE_JS_PROLOG                                                                                             \
    "function <class>(url) {\n\
    this.url = url;\n\
    var id = 1;\n\
    \n\
    function doJsonRpcRequest(method, params, methodCall, callback_success, callback_error) {\n\
        var request = {};\n\
        if (methodCall)\n\
            request.id = id++;\n\
        request.jsonrpc = \"2.0\";\n\
        request.method = method;\n\
        if (params !== null) {\n\
            request.params = params;\n\
        }\n\
        JSON.stringify(request);\n\
        \n\
        $.ajax({\n\
            type: \"POST\",\n\
            url: url,\n\
            data: JSON.stringify(request),\n\
            success: function (response) {\n\
                if (methodCall) {\n\
                    if (response.hasOwnProperty(\"result\") && response.hasOwnProperty(\"id\")) {\n\
                        callback_success(response.id, response.result);\n\
                    } else if (response.hasOwnProperty(\"error\")) {\n\
                        if (callback_error != null)\n\
                            callback_error(response.error.code,response.error.message);\n\
                    } else {\n\
                        if (callback_error != null)\n\
                            callback_error(-32001, \"Invalid Server response: \" + response);\n\
                    }\n\
                }\n\
            },\n\
            error: function () {\n\
                if (methodCall)\n\
                    callback_error(-32002, \"AJAX Error\");\n\
            },\n\
            dataType: \"json\"\n\
        });\n\
        return id-1;\n\
    }\n\
    this.doRPC = function(method, params, methodCall, callback_success, callback_error) {\n\
        return doJsonRpcRequest(method, params, methodCall, callback_success, callback_error);\n\
    }\n\
}\n"

extern const char* TEMPLATE_JS_METHOD;
extern const char* TEMPLATE_JS_PARAM_NAMED;
extern const char* TEMPLATE_JS_PARAM_POSITIONAL;
extern const char* TEMPLATE_JS_PARAM_EMPTY;
extern const char* TEMPLATE_JS_CALL_METHOD;

void JsClientCodeGenerator::generateStub() {
    writeLine("/**");
    writeLine(" * This file is generated by jsonrpcstub, DO NOT CHANGE IT MANUALLY!");
    writeLine(" */");
    write(substitute(TEMPLATE_JS_PROLOG, "<class>", stubname));
    writeNewLine();

    for (unsigned int i = 0; i < procedures.size(); i++) {
        generateMethod(procedures[i]);
    }
}

void JsClientCodeGenerator::generateMethod(Procedure& proc) {
    string method = TEMPLATE_JS_METHOD;
    replaceAll(method, "<class>", stubname);
    replaceAll(method, "<procedure>", noramlizeJsLiteral(proc.GetProcedureName()));

    stringstream param_string;
    stringstream params_assignment;

    parameterNameList_t list = proc.GetParameters();
    for (parameterNameList_t::iterator it = list.begin(); it != list.end();) {
        param_string << it->first;

        if (proc.GetParameterDeclarationType() == PARAMS_BY_NAME)
            params_assignment << it->first << " : " << it->first;
        else
            params_assignment << it->first;

        if (++it != list.end()) {
            params_assignment << ", ";
        }
        param_string << ", ";
    }

    replaceAll(method, "<params>", param_string.str());

    writeLine(method);
    increaseIndentation();

    string params;

    if (proc.GetParameters().size() > 0) {
        if (proc.GetParameterDeclarationType() == PARAMS_BY_NAME)
            params = TEMPLATE_JS_PARAM_NAMED;
        else
            params = TEMPLATE_JS_PARAM_POSITIONAL;

        replaceAll(params, "<params>", params_assignment.str());
        writeLine(params);
    } else {
        writeLine(TEMPLATE_JS_PARAM_EMPTY);
    }

    method = TEMPLATE_JS_CALL_METHOD;

    replaceAll(method, "<procedure>", proc.GetProcedureName());

    writeLine(method);

    decreaseIndentation();
    writeLine("};");
}

string JsClientCodeGenerator::noramlizeJsLiteral(const string& literal) {
    string result = literal;
    for (unsigned int i = 0; i < literal.length(); i++) {
        if (!((literal[i] >= 'a' && literal[i] <= 'z') || (literal[i] >= 'A' && literal[i] <= 'Z') ||
              (literal[i] >= '0' && literal[i] <= '9') || literal[i] == '_')) {
            result[i] = '_';
        }
    }
    return result;
}

const char* TEMPLATE_JS_METHOD = "<class>.prototype.<procedure> = function(<params>callbackSuccess, callbackError) {";
const char* TEMPLATE_JS_PARAM_NAMED = "var params = {<params>};";
const char* TEMPLATE_JS_PARAM_POSITIONAL = "var params = [<params>];";
const char* TEMPLATE_JS_PARAM_EMPTY = "var params = null;";
const char* TEMPLATE_JS_CALL_METHOD =
    "return this.doRPC(\"<procedure>\", params, true, callbackSuccess, callbackError);";
