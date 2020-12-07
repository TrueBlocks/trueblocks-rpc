/*-----------------------------------------------------------------------
 * This file is part of TrueBlocks-RPC, which is a near-complete rewrite
 * of libjson-rpc-cpp removing anything not directly needed to support
 * an Ethereum RPC client and server. It retains the original license
 * as described in LICENSE.txt
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @author  Thomas Jay Rush <jrush@quickblocks.io> (rewrite 2021)
 *---------------------------------------------------------------------*/
#pragma once

#include <string>
#include <ostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <rpclib/procedure.h>

extern string toCppReturnType(jsontype_t type);
extern string toCppParamType(jsontype_t type);
namespace jsonrpc {
    //---------------------------------------------------------------------------------------
    extern void splitPackages(const string& classname, StringVector& result);
    extern string normalize(const string& text);
    extern string toString(jsontype_t type);

    //---------------------------------------------------------------------------------------
    class CodeGenerator {
      public:
        CodeGenerator(const string& sn, ProcedureVector& pv, const string& filename)
            : stubname(sn), procedures(pv), indChar(' '), indCount(4), indLevel(0), atBeginning(true) {
            file.open(filename.c_str());
            output = &file;
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
                for (int i = 0; i < indLevel; i++)
                    *output << string(indCount, indChar);
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
            indLevel++;
        }

        void decreaseIndentation() {
            indLevel--;
        }

        void setIndentSymbol(char ch, int n = 4) {
            indCount = n;
            indChar = ch;
        }

        string indent(int inc = 0) {
            indLevel += inc;
            return string(indCount * indLevel, indChar);
        }
        string indentIn(void) {
            return indent(1);
        }
        string indentOut(void) {
            return indent(-1);
        }

        virtual void generateStub() = 0;

        void namespaceClose(int depth) {
            for (int i = 0; i < depth; i++) {
                decreaseIndentation();
                writeLine("}");
            }
            writeLine("");
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
        std::ostream* output;
        string stubname;
        ProcedureVector& procedures;
        std::ofstream file;
        char indChar;
        int indCount;
        int indLevel;
        bool atBeginning;
    };

    //---------------------------------------------------------------------------------------
    class PythonClientCodeGenerator : public CodeGenerator {
      public:
        PythonClientCodeGenerator(const string& stubname, ProcedureVector& procedures, const string filename)
            : CodeGenerator(stubname, procedures, filename) {
        }

        void generateStub() final;

      private:
        void generateMethod(Procedure& proc);
        void generateAssignments(Procedure& proc);
    };

    //---------------------------------------------------------------------------------------
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

    //---------------------------------------------------------------------------------------
    class CppClientCodeGenerator : public CodeGenerator {
      public:
        CppClientCodeGenerator(const string& stubname, ProcedureVector& procedures, const string filename)
            : CodeGenerator(stubname, procedures, filename) {
        }
        void generateStub(void) final;
    };

    class CppServerCodeGenerator : public CodeGenerator {
      public:
        CppServerCodeGenerator(const string& stubname, ProcedureVector& procedures, const string& filename)
            : CodeGenerator(stubname, procedures, filename) {
        }
        void generateStub();
    };
}  // namespace jsonrpc
