/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    generate.h
 * @date    3/21/2014
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/
#pragma once

#include <string>
#include <ostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <rpct/procedure.h>

namespace jsonrpc {
    extern void splitPackages(const string& classname, StringVector& result);
    extern string normalizeString(const string& text);
    extern string toString(jsontype_t type);
    extern string toCppType(jsontype_t type, bool isReturn = false);

    class CodeGenerator {
      public:
        CodeGenerator(const string& sn, ProcedureVector& pv, const string& filename)
            : stubname(sn), procedures(pv), indentation(0), atBeginning(true) {
            file.open(filename.c_str());
            output = &file;
            indentSymbol = "    ";
        }
        virtual ~CodeGenerator() {
            output->flush();
            if (file.is_open()) {
                file.close();
            }
        }

        void write(const string& line) {
            if (atBeginning) {
                atBeginning = false;
                for (int i = 0; i < indentation; i++)
                    *output << indentSymbol;
            }
            *output << line;
        }

        void writeLine(const string& line) {
            write(line);
            writeNewLine();
        }

        void writeNewLine() {
            *output << std::endl;
            atBeginning = true;
        }

        void increaseIndentation() {
            indentation++;
        }

        void decreaseIndentation() {
            indentation--;
        }

        void setIndentSymbol(const string& symbol) {
            indentSymbol = symbol;
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

      private:
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
        void generateMethod(Procedure& proc);
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

        void generateStub();

      private:
        void generateBindings();
        void generateProcedureDefinitions();
        void generateAbstractDefinitions();
        void generateParameterMapping(const Procedure& proc);
        string generateBindingParameterlist(const Procedure& proc);
    };
}  // namespace jsonrpc
