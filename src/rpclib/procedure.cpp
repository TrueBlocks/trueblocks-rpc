/*-----------------------------------------------------------------------
 * This file was originally part of libjson-rpc-cpp which has been
 * almost completely re-written to remove anything not directly needed
 * by the Ethereum RPC. It retains the original license as described in
 * LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite circa 2020)
 *---------------------------------------------------------------------*/
#include <cstdarg>
#include <vector>

#include <rpclib/procedure.h>
#include <rpclib/errors.h>
#include <rpclib/exception.h>

using namespace std;
using namespace jsonrpc;

//---------------------------------------------------------------------------------------
Procedure::Procedure() : procedureName(""), returntype(JSON_BOOLEAN), paramDeclaration(PARAMS_BY_NAME) {
}

//---------------------------------------------------------------------------------------
Procedure::Procedure(const string& name, parameterDeclaration_t paramType, jsontype_t returntype, ...) {
    va_list parameters;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wvarargs"
    va_start(parameters, returntype);
#pragma clang diagnostic pop
    const char* paramname = va_arg(parameters, const char*);
    jsontype_t type;
    while (paramname != NULL) {
        type = (jsontype_t)va_arg(parameters, int);
        AddParameter(paramname, type);
        paramname = va_arg(parameters, const char*);
    }
    va_end(parameters);
    procedureName = name;
    returntype = returntype;
    paramDeclaration = paramType;
}

//---------------------------------------------------------------------------------------
bool Procedure::ValdiateParameters(const Json::Value& parameters) const {
    if (parametersName.empty()) {
        return true;
    }
    if (parameters.isArray() && paramDeclaration == PARAMS_BY_POSITION) {
        return ValidatePositionalParameters(parameters);
    } else if (parameters.isObject() && paramDeclaration == PARAMS_BY_NAME) {
        return ValidateNamedParameters(parameters);
    } else {
        return false;
    }
}

//---------------------------------------------------------------------------------------
const parameterNameList_t& Procedure::GetParameters() const {
    return parametersName;
}

//---------------------------------------------------------------------------------------
const string& Procedure::GetProcedureName() const {
    return procedureName;
}

//---------------------------------------------------------------------------------------
parameterDeclaration_t Procedure::GetParameterDeclarationType() const {
    return paramDeclaration;
}

//---------------------------------------------------------------------------------------
jsontype_t Procedure::GetReturnType() const {
    return returntype;
}

//---------------------------------------------------------------------------------------
void Procedure::SetProcedureName(const string& name) {
    procedureName = name;
}

//---------------------------------------------------------------------------------------
void Procedure::SetReturnType(jsontype_t type) {
    returntype = type;
}

//---------------------------------------------------------------------------------------
void Procedure::SetParameterDeclarationType(parameterDeclaration_t type) {
    paramDeclaration = type;
}

//---------------------------------------------------------------------------------------
void Procedure::AddParameter(const string& name, jsontype_t type) {
    parametersName[name] = type;
    parametersPosition.push_back(type);
}

//---------------------------------------------------------------------------------------
bool Procedure::ValidateNamedParameters(const Json::Value& parameters) const {
    bool ok = parameters.isObject() || parameters.isNull();
    for (map<string, jsontype_t>::const_iterator it = parametersName.begin(); ok == true && it != parametersName.end();
         ++it) {
        if (!parameters.isMember(it->first)) {
            ok = false;
        } else {
            ok = ValidateSingleParameter(it->second, parameters[it->first]);
        }
    }
    return ok;
}

//---------------------------------------------------------------------------------------
bool Procedure::ValidatePositionalParameters(const Json::Value& parameters) const {
    bool ok = true;

    if (parameters.size() != parametersPosition.size()) {
        return false;
    }

    for (unsigned int i = 0; ok && i < parametersPosition.size(); i++) {
        ok = ValidateSingleParameter(parametersPosition.at(i), parameters[i]);
    }
    return ok;
}

//---------------------------------------------------------------------------------------
bool Procedure::ValidateSingleParameter(jsontype_t expectedType, const Json::Value& value) const {
    bool ok = true;
    switch (expectedType) {
        case JSON_STRING:
            if (!value.isString())
                ok = false;
            break;
        case JSON_BOOLEAN:
            if (!value.isBool())
                ok = false;
            break;
        case JSON_INTEGER:
            if (!value.isIntegral())
                ok = false;
            break;
        case JSON_REAL:
            if (!value.isDouble())
                ok = false;
            break;
        case JSON_NUMERIC:
            if (!value.isNumeric())
                ok = false;
            break;
        case JSON_OBJECT:
            if (!value.isObject())
                ok = false;
            break;
        case JSON_ARRAY:
            if (!value.isArray())
                ok = false;
            break;
    }
    return ok;
}
