/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    generate.h
 * @date    3/21/2014
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef JSONRPC_CPP_CODEGENERATOR_H
#define JSONRPC_CPP_CODEGENERATOR_H

#include <string>
#include <ostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <rpct/common/procedure.h>

namespace jsonrpc {
    inline void splitPackages(const string& classname, StringVector& result) {
        string s = classname;
        string delimiter = "::";
        size_t pos = 0;
        while ((pos = s.find(delimiter)) != string::npos) {
            string str = s.substr(0, pos);
            result.push_back(str);
            s.erase(0, pos + delimiter.length());
        }
        result.push_back(s);
    }

    inline string normalizeString(const string& text) {
        string result = text;
        for (unsigned int i = 0; i < text.length(); i++) {
            if (!((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z') ||
                  (text[i] >= '0' && text[i] <= '9') || text[i] == '_')) {
                result[i] = '_';
            }
        }
        return result;
    }

    inline string toString(jsontype_t type) {
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

    inline string toCppType(jsontype_t type, bool isReturn = false) {
        string result;
        switch (type) {
            case JSON_BOOLEAN:
                result = "bool";
                break;
            case JSON_INTEGER:
                result = "int";
                break;
            case JSON_REAL:
                result = "double";
                break;
            case JSON_NUMERIC:
                result = "double";
                break;
            case JSON_STRING:
                result = isReturn ? "string" : "const string&";
                break;
            default:
                result = isReturn ? "Json::Value" : "const Json::Value&";
                break;
        }
        return result;
    }

    class CodeGenerator {
      public:
        CodeGenerator(const string& sn, ProcedureVector& pv, const string& filename)
            : stubname(sn), procedures(pv), indentation(0), atBeginning(true) {
            this->file.open(filename.c_str());
            this->output = &this->file;
            this->indentSymbol = "    ";
        }
        virtual ~CodeGenerator() {
            this->output->flush();
            if (this->file.is_open()) {
                this->file.close();
            }
        }

        void write(const string& line) {
            if (this->atBeginning) {
                this->atBeginning = false;
                for (int i = 0; i < this->indentation; i++)
                    *this->output << this->indentSymbol;
            }
            *this->output << line;
        }

        void writeLine(const string& line) {
            this->write(line);
            this->writeNewLine();
        }

        void writeNewLine() {
            *this->output << std::endl;
            this->atBeginning = true;
        }

        void increaseIndentation() {
            this->indentation++;
        }

        void decreaseIndentation() {
            this->indentation--;
        }

        void setIndentSymbol(const string& symbol) {
            this->indentSymbol = symbol;
        }

        virtual string generateParameterDeclarationList(Procedure& proc) {
            stringstream param_string;
            parameterNameList_t list = proc.GetParameters();
            for (parameterNameList_t::iterator it = list.begin(); it != list.end();) {
                param_string << toCppType(it->second) << " " << it->first;
                if (++it != list.end()) {
                    param_string << ", ";
                }
            }
            return param_string.str();
        }

        virtual void generateStub() = 0;

        void namespaceClose(int depth) {
            for (int i = 0; i < depth; i++) {
                decreaseIndentation();
                writeLine("}");
            }
        }

        int namespaceOpen(const string& classname) {
            StringVector namespaces;
            splitPackages(classname, namespaces);
            for (unsigned int i = 0; i < namespaces.size() - 1; i++) {
                write("namespace ");
                write(namespaces.at(i));
                writeLine(" {");
                increaseIndentation();
            }
            return namespaces.size() - 1;
        }

      protected:
        string stubname;
        ProcedureVector& procedures;

      private:
        std::ostream* output;
        std::ofstream file;
        string indentSymbol;
        int indentation;
        bool atBeginning;
    };

    class PythonClientCodeGenerator : public CodeGenerator {
      public:
        PythonClientCodeGenerator(const string& stubname, ProcedureVector& procedures, const string filename)
            : CodeGenerator(stubname, procedures, filename) {
        }

        void generateStub() final;
        void generateMethod(Procedure& proc);
        void generateAssignments(Procedure& proc);
        void generateProcCall(Procedure& proc);
        string generateParameterDeclarationList(Procedure& proc) {
            stringstream param_string;
            parameterNameList_t list = proc.GetParameters();
            for (parameterNameList_t::iterator it = list.begin(); it != list.end(); ++it) {
                param_string << ", ";
                param_string << it->first;
            }
            return param_string.str();
        }
    };

    class JsClientCodeGenerator : public CodeGenerator {
      public:
        JsClientCodeGenerator(const string& stubname, ProcedureVector& procedures, const string& filename)
            : CodeGenerator(stubname, procedures, filename) {
        }

        void generateStub() final;

      private:
        virtual void generateMethod(Procedure& proc);
        static string noramlizeJsLiteral(const string& literal);
    };

    class CppClientCodeGenerator : public CodeGenerator {
      public:
        CppClientCodeGenerator(const string& stubname, ProcedureVector& procedures, const string filename)
            : CodeGenerator(stubname, procedures, filename) {
        }
        void generateStub(void) final;

      private:
        void generateMethod(Procedure& proc);
        void generateAssignments(Procedure& proc);
        void generateProcCall(Procedure& proc);
    };

    class CppServerCodeGenerator : public CodeGenerator {
      public:
        CppServerCodeGenerator(const string& stubname, ProcedureVector& procedures, const string& filename)
            : CodeGenerator(stubname, procedures, filename) {
        }

        virtual void generateStub();

        void generateBindings();
        void generateProcedureDefinitions();
        void generateAbstractDefinitions();
        string generateBindingParameterlist(const Procedure& proc);
        void generateParameterMapping(const Procedure& proc);
    };
}  // namespace jsonrpc

#endif  // JSONRPC_CPP_CODEGENERATOR_H
