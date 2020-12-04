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
#include <json/json.h>

#include <rpclib/utils.h>

namespace jsonrpc {

    //---------------------------------------------------------------------------------------
    class Procedure {
      public:
        Procedure();
        Procedure(const string& name, parameterDeclaration_t paramType, jsontype_t returntype, ...);

        bool ValdiateParameters(const Json::Value& parameters) const;
        const parameterNameList_t& GetParameters() const;
        const string& GetProcedureName() const;
        jsontype_t GetReturnType() const;
        parameterDeclaration_t GetParameterDeclarationType() const;
        void SetProcedureName(const string& name);
        void SetReturnType(jsontype_t type);
        void SetParameterDeclarationType(parameterDeclaration_t type);
        void AddParameter(const string& name, jsontype_t type);
        bool ValidateNamedParameters(const Json::Value& parameters) const;
        bool ValidatePositionalParameters(const Json::Value& parameters) const;

      private:
        string procedureName;
        parameterNameList_t parametersName;
        parameterPositionList_t parametersPosition;
        jsontype_t returntype;
        parameterDeclaration_t paramDeclaration;
        bool ValidateSingleParameter(jsontype_t expectedType, const Json::Value& value) const;
    };
    typedef std::vector<Procedure> ProcedureVector;
} /* namespace jsonrpc */
