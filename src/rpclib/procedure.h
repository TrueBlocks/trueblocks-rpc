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

    class Procedure {
      public:
        Procedure();

        /**
         * @brief Constructor for method with parameters as va_list. The last
         * parameter must be NULL. If no parameters are passed, parameters either do
         * not exist, or cannot be checked for type compliance by the library.
         * @param name
         * @param returntype
         */
        Procedure(const string& name, parameterDeclaration_t paramType, jsontype_t returntype, ...);

        /**
         * This method is validating the incoming parameters for each procedure.
         * @param parameters - should contain the parameter-object of an valid
         * json-rpc 2.0 request
         * @see http://groups.google.com/group/json-rpc/web/json-rpc-2-0
         * @return true on successful validation false otherwise.
         *
         * If the valid parameters are of Type JSON_ARRAY or JSON_OBJECT, they can
         * only be checked for name and not for their structure.
         */
        bool ValdiateParameters(const Json::Value& parameters) const;

        // Various get methods.
        const parameterNameList_t& GetParameters() const;
        const string& GetProcedureName() const;
        jsontype_t GetReturnType() const;
        parameterDeclaration_t GetParameterDeclarationType() const;

        // Various set methods.
        void SetProcedureName(const string& name);
        void SetReturnType(jsontype_t type);
        void SetParameterDeclarationType(parameterDeclaration_t type);

        /**
         * @brief AddParameter
         * @param name describes the name of the parameter. In case of an positional
         * parameters, this value can be anything.
         * @param type describes the defined type for this parameter.
         */
        void AddParameter(const string& name, jsontype_t type);

        bool ValidateNamedParameters(const Json::Value& parameters) const;
        bool ValidatePositionalParameters(const Json::Value& parameters) const;

      private:
        /**
         * Each Procedure should have a name.
         */
        string procedureName;

        /**
         * This map represents all necessary Parameters of each Procedure.
         * The string represents the name of each parameter and JsonType the type it
         * should have.
         */
        parameterNameList_t parametersName;

        /**
         * This vector holds all parametertypes by position.
         */
        parameterPositionList_t parametersPosition;

        /**
         * @brief this field is only valid if procedure is of type method.
         */
        jsontype_t returntype;

        /**
         * @brief paramDeclaration this field defines if procedure uses named or
         * positional parameters.
         */
        parameterDeclaration_t paramDeclaration;

        bool ValidateSingleParameter(jsontype_t expectedType, const Json::Value& value) const;
    };
    typedef std::vector<Procedure> ProcedureVector;
} /* namespace jsonrpc */
