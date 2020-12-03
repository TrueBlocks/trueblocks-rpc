#include "generate.h"
#include <algorithm>

extern const char* TEMPLATE_PYTHON_CLIENT_SIGCLASS;
extern const char* TEMPLATE_PYTHON_CLIENT_SIGCONSTRUCTOR;
extern const char* TEMPLATE_PYTHON_CLIENT_SIGMETHOD;
extern const char* TEMPLATE_PYTHON_NAMED_ASSIGNMENT;
extern const char* TEMPLATE_PYTHON_POSITION_ASSIGNMENT;
extern const char* TEMPLATE_PYTHON_METHODCALL;
extern const char* TEMPLATE_PYTHON_NOTIFICATIONCALL;

using namespace std;
using namespace jsonrpc;

void PythonClientCodeGenerator::generateStub() {
    this->writeLine("#");
    this->writeLine("# This file is generated by jsonrpcstub, DO NOT CHANGE IT MANUALLY!");
    this->writeLine("#");
    this->writeNewLine();
    this->writeLine("#");
    this->writeLine("# To use this client, jsonrpc_pyclient must be installed:");
    this->writeLine("# pip install jsonrpc_pyclient");
    this->writeLine("#");
    this->writeNewLine();
    this->writeLine("from jsonrpc_pyclient import client");
    this->writeNewLine();

    this->writeLine(substitute(TEMPLATE_PYTHON_CLIENT_SIGCLASS, "<stubname>", this->stubname));
    this->increaseIndentation();

    this->writeLine(substitute(TEMPLATE_PYTHON_CLIENT_SIGCONSTRUCTOR, "<stubname>", this->stubname));
    this->writeNewLine();

    for (unsigned int i = 0; i < procedures.size(); i++) {
        this->generateMethod(procedures[i]);
    }

    this->decreaseIndentation();
    this->writeNewLine();
}

void PythonClientCodeGenerator::generateMethod(Procedure& proc) {
    string procsignature = TEMPLATE_PYTHON_CLIENT_SIGMETHOD;
    replaceAll(procsignature, "<methodname>", normalizeString(proc.GetProcedureName()));

    // generate parameters string
    string params = generateParameterDeclarationList(proc);
    replaceAll(procsignature, "<parameters>", params);

    this->writeLine(procsignature);
    this->increaseIndentation();

    generateAssignments(proc);
    this->writeNewLine();
    generateProcCall(proc);
    this->writeNewLine();

    this->decreaseIndentation();
}

void PythonClientCodeGenerator::generateAssignments(Procedure& proc) {
    string assignment;
    parameterNameList_t list = proc.GetParameters();
    if (list.size() > 0) {
        parameterDeclaration_t declType = proc.GetParameterDeclarationType();
        if (proc.GetParameterDeclarationType() == PARAMS_BY_NAME) {
            this->writeLine("parameters = {}");
        } else if (proc.GetParameterDeclarationType() == PARAMS_BY_POSITION) {
            this->writeLine("parameters = []");
        }

        for (parameterNameList_t::iterator it = list.begin(); it != list.end(); ++it) {
            if (declType == PARAMS_BY_NAME) {
                assignment = TEMPLATE_PYTHON_NAMED_ASSIGNMENT;
            } else {
                assignment = TEMPLATE_PYTHON_POSITION_ASSIGNMENT;
            }
            replaceAll(assignment, "<paramname>", it->first);
            this->writeLine(assignment);
        }
    } else {
        this->writeLine("parameters = None");
    }
}

void PythonClientCodeGenerator::generateProcCall(Procedure& proc) {
    string call;
    if (proc.GetProcedureType() == RPC_METHOD) {
        call = TEMPLATE_PYTHON_METHODCALL;
        this->writeLine(substitute(call, "<name>", proc.GetProcedureName()));
        this->writeLine("return result");
    } else {
        call = TEMPLATE_PYTHON_NOTIFICATIONCALL;
        replaceAll(call, "<name>", proc.GetProcedureName());
        this->writeLine(call);
    }
}

const char* TEMPLATE_PYTHON_CLIENT_SIGCLASS = "class <stubname>(client.Client):";

const char* TEMPLATE_PYTHON_CLIENT_SIGCONSTRUCTOR =
    "def __init__(self, connector, version='2.0'):\n        super(<stubname>, self).__init__(connector, version)";

const char* TEMPLATE_PYTHON_CLIENT_SIGMETHOD = "def <methodname>(self<parameters>):";

const char* TEMPLATE_PYTHON_NAMED_ASSIGNMENT = "parameters[\'<paramname>\'] = <paramname>";
const char* TEMPLATE_PYTHON_POSITION_ASSIGNMENT = "parameters.append(<paramname>)";

const char* TEMPLATE_PYTHON_METHODCALL = "result = self.call_method(\'<name>\', parameters)";
const char* TEMPLATE_PYTHON_NOTIFICATIONCALL = "self.call_notification(\'<name>\', parameters)";