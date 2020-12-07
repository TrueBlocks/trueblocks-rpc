/*-----------------------------------------------------------------------
 * This file is part of TrueBlocks-RPC, which is a near-complete rewrite
 * of libjson-rpc-cpp removing anything not directly needed to support
 * an Ethereum RPC client and server. It retains the original license
 * as described in LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite 2021)
 *---------------------------------------------------------------------*/
#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <argtable2.h>

#include <rpclib/version.h>
#include <rpclib/exception.h>
#include <rpclib/procedure.h>
#include <gen/gen.h>

namespace jsonrpc {
    //---------------------------------------------------------------------------------------
    void splitPackages(const string& stubname, StringVector& result) {
        string s = stubname;
        string delimiter = "::";
        size_t pos = 0;
        while ((pos = s.find(delimiter)) != string::npos) {
            string str = s.substr(0, pos);
            result.push_back(str);
            s.erase(0, pos + delimiter.length());
        }
        result.push_back(s);
    }

    //---------------------------------------------------------------------------------------
    string normalize(const string& text) {
        string result = text;
        for (unsigned int i = 0; i < text.length(); i++) {
            if (!((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z') ||
                  (text[i] >= '0' && text[i] <= '9') || text[i] == '_')) {
                result[i] = '_';
            }
        }
        return result;
    }

    //---------------------------------------------------------------------------------------
    string toString(jsontype_t type) {
        string result;
        switch (type) {
            case JSON_BOOLEAN:
                result = "JSON_BOOLEAN";
                break;
            case JSON_INTEGER:
                result = "JSON_INTEGER";
                break;
            case JSON_REAL:
                result = "JSON_REAL";
                break;
            case JSON_NUMERIC:
                result = "JSON_NUMERIC";
                break;
            case JSON_STRING:
                result = "JSON_STRING";
                break;
            case JSON_OBJECT:
                result = "JSON_OBJECT";
                break;
            case JSON_ARRAY:
                result = "JSON_ARRAY";
                break;
        }
        return result;
    }

}  // namespace jsonrpc

using namespace jsonrpc;
using namespace std;

//---------------------------------------------------------------------------------------
string class2Filename(const string& classname, const string& ft) {
    StringVector packages;
    splitPackages(classname, packages);
    string data = packages.at(packages.size() - 1);
    transform(data.begin(), data.end(), data.begin(), ::tolower);
    return data + ft;
}

//---------------------------------------------------------------------------------------
jsontype_t toJsonType(jsonval_t& val) {
    jsontype_t result;
    switch (val.type()) {
        case Json::uintValue:
        case Json::intValue:
            result = JSON_INTEGER;
            break;
        case Json::realValue:
            result = JSON_REAL;
            break;
        case Json::stringValue:
            result = JSON_STRING;
            break;
        case Json::booleanValue:
            result = JSON_BOOLEAN;
            break;
        case Json::arrayValue:
            result = JSON_ARRAY;
            break;
        case Json::objectValue:
            result = JSON_OBJECT;
            break;
        default:
            throw JsonRpcException(ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX,
                                   "Unknown parameter type: " + val.toStyledString());
    }
    return result;
}

//---------------------------------------------------------------------------------------
static void GetFileContent(const string& filename, string& target) {
    ifstream config(filename.c_str());
    if (config) {
        config.open(filename.c_str(), ios::in);
        target.assign((std::istreambuf_iterator<char>(config)), (std::istreambuf_iterator<char>()));
    } else {
        throw JsonRpcException(ERROR_SERVER_PROCEDURE_SPECIFICATION_NOT_FOUND, filename);
    }
}

//---------------------------------------------------------------------------------------
extern const char* KEY_SPEC_PROCEDURE_NAME;
extern const char* KEY_SPEC_PROCEDURE_PARAMETERS;
extern const char* KEY_SPEC_RETURN_TYPE;

//---------------------------------------------------------------------------------------
void GetPositionalParameters(jsonval_t& val, Procedure& result) {
    for (unsigned int i = 0; i < val[KEY_SPEC_PROCEDURE_PARAMETERS].size(); i++) {
        stringstream paramname;
        paramname << "param" << std::setfill('0') << std::setw(2) << (i + 1);
        result.AddParameter(paramname.str(), toJsonType(val[KEY_SPEC_PROCEDURE_PARAMETERS][i]));
    }
}

//---------------------------------------------------------------------------------------
void GetNamedParameters(jsonval_t& val, Procedure& result) {
    StringVector parameters = val[KEY_SPEC_PROCEDURE_PARAMETERS].getMemberNames();
    for (unsigned int i = 0; i < parameters.size(); ++i) {
        result.AddParameter(parameters.at(i), toJsonType(val[KEY_SPEC_PROCEDURE_PARAMETERS][parameters.at(i)]));
    }
}

//---------------------------------------------------------------------------------------
string GetName(jsonval_t& signature) {
    if (signature[KEY_SPEC_PROCEDURE_NAME].isString())
        return signature[KEY_SPEC_PROCEDURE_NAME].asString();
    return "";
}

//---------------------------------------------------------------------------------------
void GetProcedure(jsonval_t& signature, Procedure& result) {
    if (signature.isObject() && GetName(signature) != "") {
        result.SetProcedureName(GetName(signature));
        if (signature.isMember(KEY_SPEC_RETURN_TYPE)) {
            result.SetReturnType(toJsonType(signature[KEY_SPEC_RETURN_TYPE]));
        }
        if (signature.isMember(KEY_SPEC_PROCEDURE_PARAMETERS)) {
            if (signature[KEY_SPEC_PROCEDURE_PARAMETERS].isObject() ||
                signature[KEY_SPEC_PROCEDURE_PARAMETERS].isArray()) {
                if (signature[KEY_SPEC_PROCEDURE_PARAMETERS].isArray()) {
                    result.SetParameterDeclarationType(PARAMS_BY_POSITION);
                    GetPositionalParameters(signature, result);
                } else if (signature[KEY_SPEC_PROCEDURE_PARAMETERS].isObject()) {
                    result.SetParameterDeclarationType(PARAMS_BY_NAME);
                    GetNamedParameters(signature, result);
                }
            } else {
                throw JsonRpcException(ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX,
                                       "Invalid signature types in fileds: " + signature.toStyledString());
            }
        }
    } else {
        throw JsonRpcException(
            ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX,
            "procedure declaration does not contain name or parameters: " + signature.toStyledString());
    }
}

//---------------------------------------------------------------------------------------
ProcedureVector GetProceduresFromString(const string& content) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    Json::Reader reader;
#pragma clang diagnostic pop
    jsonval_t val;
    if (!reader.parse(content, val)) {
        throw JsonRpcException(ERROR_RPC_JSON_PARSE_ERROR, " specification file contains syntax errors");
    }

    if (!val.isArray()) {
        throw JsonRpcException(ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX, " top level json value is not an array");
    }

    ProcedureVector result;
    map<string, Procedure> procnames;
    for (unsigned int i = 0; i < val.size(); i++) {
        Procedure proc;
        GetProcedure(val[i], proc);
        if (procnames.find(proc.GetName()) != procnames.end()) {
            throw JsonRpcException(ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX,
                                   "Procedurename not unique: " + proc.GetName());
        }
        procnames[proc.GetName()] = proc;
        result.push_back(proc);
    }
    return result;
}

//---------------------------------------------------------------------------------------
ProcedureVector GetProceduresFromFile(const string& filename) {
    string content;
    GetFileContent(filename, content);
    return GetProceduresFromString(content);
}

//---------------------------------------------------------------------------------------
bool createStubGenerators(int argc, char** argv, ProcedureVector& procedures, vector<CodeGenerator*>& generators,
                          FILE* _stdout, FILE* _stderr) {
    struct arg_file* inputfile = arg_file0(NULL, NULL, "<specfile>", "path of input specification file");
    struct arg_lit* help = arg_lit0("h", "help", "print this help and exit");
    struct arg_lit* verbose = arg_lit0("v", "verbose", "print more information about what is happening");
    struct arg_lit* version = arg_lit0(NULL, "version", "print version and exit");
    struct arg_str* cppserver =
        arg_str0(NULL, "cpp-server", "<namespace::classname>", "name of the C++ server stub class");
    struct arg_str* cppserverfile =
        arg_str0(NULL, "cpp-server-file", "<filename.h>", "name of the C++ server stub file");
    struct arg_str* cppclient =
        arg_str0(NULL, "cpp-client", "<namespace::classname>", "name of the C++ client stub class");
    struct arg_str* cppclientfile =
        arg_str0(NULL, "cpp-client-file", "<filename.h>", "name of the C++ client stub file");
    struct arg_str* jsclient = arg_str0(NULL, "js-client", "<classname>", "name of the JavaScript client stub class");
    struct arg_str* jsclientfile =
        arg_str0(NULL, "js-client-file", "<filename.js>", "name of the JavaScript client stub file");
    struct arg_str* pyclient = arg_str0(NULL, "py-client", "<classname>", "name of the Python client stub class");
    struct arg_str* pyclientfile =
        arg_str0(NULL, "py-client-file", "<filename.py>", "name of the Python client stub file");
    struct arg_end* end = arg_end(20);

    void* argtable[] = {inputfile, help,          version,  verbose,      cppserver, cppserverfile,
                        cppclient, cppclientfile, jsclient, jsclientfile, pyclient,  pyclientfile,
                        end};

    if (arg_parse(argc, argv, argtable) > 0) {
        arg_print_errors(_stderr, end, argv[0]);
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return false;
    }

    if (help->count > 0) {
        fprintf(_stdout, "Usage: %s ", argv[0]);
        arg_print_syntax(_stdout, argtable, "\n");
        cout << endl;
        arg_print_glossary_gnu(_stdout, argtable);
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return true;
    }

    if (version->count > 0) {
        fprintf(_stdout, "jsonrpcstub version %d.%d.%d\n", JSONRPC_CPP_MAJOR_VERSION, JSONRPC_CPP_MINOR_VERSION,
                JSONRPC_CPP_PATCH_VERSION);
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return true;
    }

    if (inputfile->count == 0) {
        fprintf(_stderr, "Invalid arguments: specfile must be provided.\n");
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return false;
    }

    try {
        procedures = GetProceduresFromFile(inputfile->filename[0]);
        if (verbose->count > 0) {
            fprintf(_stdout, "Found %zu procedures in %s\n", procedures.size(), inputfile->filename[0]);
            for (unsigned int i = 0; i < procedures.size(); ++i) {
                fprintf(_stdout, "\t[Method]         ");
                fprintf(_stdout, "%s\n", procedures.at(i).GetName().c_str());
            }
            fprintf(_stdout, "\n");
        }

        if (cppserver->count > 0) {
            string filename;
            if (cppserverfile->count > 0)
                filename = cppserverfile->sval[0];
            else
                filename = class2Filename(cppserver->sval[0], ".h");
            if (verbose->count > 0)
                fprintf(_stdout, "Generating C++ Serverstub to: %s\n", filename.c_str());
            generators.push_back(new CppServerCodeGenerator(cppserver->sval[0], procedures, filename));
        }

        if (cppclient->count > 0) {
            string filename;
            if (cppclientfile->count > 0)
                filename = cppclientfile->sval[0];
            else
                filename = class2Filename(cppclient->sval[0], ".h");
            if (verbose->count > 0)
                fprintf(_stdout, "Generating C++ Clientstub to: %s\n", filename.c_str());
            generators.push_back(new CppClientCodeGenerator(cppclient->sval[0], procedures, filename));
        }

        if (jsclient->count > 0) {
            string filename;
            if (jsclientfile->count > 0)
                filename = jsclientfile->sval[0];
            else
                filename = class2Filename(jsclient->sval[0], ".js");

            if (verbose->count > 0)
                fprintf(_stdout, "Generating JavaScript Clientstub to: %s\n", filename.c_str());
            generators.push_back(new JsClientCodeGenerator(jsclient->sval[0], procedures, filename));
        }

        if (pyclient->count > 0) {
            string filename;
            if (pyclientfile->count > 0)
                filename = pyclientfile->sval[0];
            else
                filename = class2Filename(pyclient->sval[0], ".py");

            if (verbose->count > 0)
                fprintf(_stdout, "Generating Python Clientstub to: %s\n", filename.c_str());
            generators.push_back(new PythonClientCodeGenerator(pyclient->sval[0], procedures, filename));
        }
    } catch (const JsonRpcException& ex) {
        fprintf(_stderr, "%s\n", ex.what());
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return false;
    }
    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    return true;
}

//---------------------------------------------------------------------------------------
int main(int argc, char** argv) {
    vector<CodeGenerator*> stubgens;
    ProcedureVector procedures;

    bool result = createStubGenerators(argc, argv, procedures, stubgens, stdout, stderr);
    for (unsigned int i = 0; i < stubgens.size(); ++i) {
        stubgens[i]->generateStub();
    }

    for (unsigned int i = 0; i < stubgens.size(); ++i) {
        delete stubgens[i];
    }

    return !result;
}

//---------------------------------------------------------------------------------------
const char* KEY_SPEC_PROCEDURE_NAME = "name";
const char* KEY_SPEC_PROCEDURE_PARAMETERS = "params";
const char* KEY_SPEC_RETURN_TYPE = "returns";
