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
#include <json/json.h>

#include <rpclib/utils.h>

namespace jsonrpc {

    //---------------------------------------------------------------------------------------
    class Procedure {
      public:
        Procedure();
        Procedure(const string& name, param_t paramType, jsontype_t returntype, ...);

        const parameterNameList_t& GetParameters() const;
        const string& GetProcedureName() const;
        jsontype_t GetReturnType() const;
        param_t GetParameterDeclarationType() const;

        void SetProcedureName(const string& name);
        void SetReturnType(jsontype_t type);
        void SetParameterDeclarationType(param_t type);

        void AddParameter(const string& name, jsontype_t type);
        bool ValdiateParameters(const jsonval_t& parameters) const;

      private:
        bool ValidateSingleParameter(jsontype_t expectedType, const jsonval_t& value) const;
        bool ValidateNamedParameters(const jsonval_t& parameters) const;
        bool ValidatePositionalParameters(const jsonval_t& parameters) const;

        string procedureName;
        parameterNameList_t parametersName;
        parameterPositionList_t parametersPosition;
        jsontype_t returntype;
        param_t paramDeclaration;
    };
    typedef std::vector<Procedure> ProcedureVector;
} /* namespace jsonrpc */
