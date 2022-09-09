#include <iostream>
#include <vector>

#include "Command.h"

using namespace std;
class CodeWriter {
 public:
  CodeWriter(const vector<Command>& commands, ostream& outputStream,
             const string programName)
      : commands(commands),
        outputStream(outputStream),
        programName(programName) {}
  void writeAsm() {
    for (Command command : commands) {
      outputStream << "// " << command.vm << endl;
      switch (command.command) {
        case C_ARITHMETIC:
          writeArithmetic(command.arg1);
          break;
        case C_POP:
          writePushPop(command);
          break;
        case C_PUSH:
          writePushPop(command);
          break;
        default:
          break;
      }
      outputStream << endl;
    }
  }

 private:
  vector<Command> commands;
  ostream& outputStream;
  string programName;
  int symbolCounter = 0;
  void writeArithmetic(string type) {
    if (type == "add") {
      writeAdd();
    }
    if (type == "sub") {
      writeSub();
    }
    if (type == "neg") {
      writeNeg();
    }
    if (type == "eq") {
      writeEq();
    }
    if (type == "gt") {
      writeGt();
    }
    if (type == "lt") {
      writeLt();
    }
    if (type == "and") {
      writeAnd();
    }
    if (type == "or") {
      writeOr();
    }
    if (type == "not") {
      writeNot();
    }
  }
  void writeAdd() {
    outputStream << "@SP" << endl;
    outputStream << "M=M-1" << endl;
    outputStream << "A=M" << endl;
    outputStream << "D=M" << endl;
    outputStream << "A=A-1" << endl;
    outputStream << "M=M+D" << endl;
  }
  void writeSub() {
    outputStream << "@SP" << endl;
    outputStream << "M=M-1" << endl;
    outputStream << "A=M" << endl;
    outputStream << "D=M" << endl;
    outputStream << "A=A-1" << endl;
    outputStream << "M=M-D" << endl;
  }

  void writeNeg() {
    outputStream << "@SP" << endl;
    outputStream << "A=M-1" << endl;
    outputStream << "M=-M" << endl;
  }

  void writeEq() {
    string symbol = "JEQ_" + to_string(symbolCounter);
    outputStream << "@SP" << endl;
    outputStream << "AM=M-1" << endl;
    outputStream << "D=M" << endl;
    outputStream << "@SP" << endl;
    outputStream << "AM=M-1" << endl;
    outputStream << "D=M-D" << endl;
    outputStream << "@" + symbol << endl;
    outputStream << "D;JEQ" << endl;
    outputStream << "D=1" << endl;
    outputStream << "(" + symbol + ")" << endl;
    outputStream << "D=D-1" << endl;
    outputStream << "@SP" << endl;
    outputStream << "A=M" << endl;
    outputStream << "M=D" << endl;
    outputStream << "@SP" << endl;
    outputStream << "M=M+1" << endl;

    symbolCounter++;
  }

  void writeGt() {
    string symbolTrue("JGT_TRUE_" + to_string(symbolCounter));
    string symbolFalse("JGT_FALSE_" + to_string(symbolCounter));

    outputStream << "@SP" << endl;
    outputStream << "AM=M-1" << endl;
    outputStream << "D=M" << endl;
    outputStream << "@SP" << endl;
    outputStream << "AM=M-1" << endl;
    outputStream << "D=M-D" << endl;
    outputStream << "@" + symbolTrue << endl;
    outputStream << "D;JGT" << endl;
    outputStream << "D=0" << endl;
    outputStream << "@" + symbolFalse << endl;
    outputStream << "0;JMP" << endl;
    outputStream << "(" + symbolTrue + ")" << endl;
    outputStream << "D=-1" << endl;
    outputStream << "(" + symbolFalse + ")" << endl;
    outputStream << "@SP" << endl;
    outputStream << "A=M" << endl;
    outputStream << "M=D" << endl;
    outputStream << "@SP" << endl;
    outputStream << "M=M+1" << endl;

    symbolCounter++;
  }

  void writeLt() {
    string symbolTrue("JLT_TRUE_" + to_string(symbolCounter));
    string symbolFalse("JLT_FALSE_" + to_string(symbolCounter));

    outputStream << "@SP" << endl;
    outputStream << "AM=M-1" << endl;
    outputStream << "D=M" << endl;
    outputStream << "@SP" << endl;
    outputStream << "AM=M-1" << endl;
    outputStream << "D=M-D" << endl;
    outputStream << "@" + symbolTrue << endl;
    outputStream << "D;JLT" << endl;
    outputStream << "D=0" << endl;
    outputStream << "@" + symbolFalse << endl;
    outputStream << "0;JMP" << endl;
    outputStream << "(" + symbolTrue + ")" << endl;
    outputStream << "D=-1" << endl;
    outputStream << "(" + symbolFalse + ")" << endl;
    outputStream << "@SP" << endl;
    outputStream << "A=M" << endl;
    outputStream << "M=D" << endl;
    outputStream << "@SP" << endl;
    outputStream << "M=M+1" << endl;

    symbolCounter++;
  }

  void writeAnd() {
    outputStream << "@SP" << endl;
    outputStream << "AM=M-1" << endl;
    outputStream << "D=M" << endl;
    outputStream << "A=A-1" << endl;
    outputStream << "M=D&M" << endl;
  }

  void writeOr() {
    outputStream << "@SP" << endl;
    outputStream << "AM=M-1" << endl;
    outputStream << "D=M" << endl;
    outputStream << "A=A-1" << endl;
    outputStream << "M=D|M" << endl;
  }

  void writeNot() {
    outputStream << "@SP" << endl;
    outputStream << "A=M" << endl;
    outputStream << "A=A-1" << endl;
    outputStream << "M=!M" << endl;
  }
  void writePushPop(Command command) {
    switch (command.command) {
      case C_PUSH:
        writePush(command.arg1, command.arg2);
        break;
      case C_POP:
        writePop(command.arg1, command.arg2);
        break;
      default:
        break;
    }
  }

  string getRegister(string segment, int index) {
    if (segment == "local") return "LCL";
    if (segment == "argument") return "ARG";
    if (segment == "this") return "THIS";
    if (segment == "that") return "THAT";
    if (segment == "pointer") return "3";
    if (segment == "temp") return "5";
    if (segment == "static") return programName + "." + to_string(index);
    return "";
  }

  void writePush(string segment, int index) {
    string indexStr = to_string(index);
    string registerStr = getRegister(segment, index);

    if (segment == "constant") {
      outputStream << "@" << indexStr << endl;
      outputStream << "D=A" << endl;
      outputStream << "@SP" << endl;
      outputStream << "M=M+1" << endl;
      outputStream << "A=M-1" << endl;
      outputStream << "M=D" << endl;
    }
    if (segment == "local" || segment == "argument" || segment == "this" ||
        segment == "that") {
      outputStream << "@" + registerStr << endl;
      outputStream << "D=M" << endl;
      outputStream << "@" + indexStr << endl;
      outputStream << "A=D+A" << endl;
      outputStream << "D=M" << endl;
      outputStream << "@SP" << endl;
      outputStream << "M=M+1" << endl;
      outputStream << "A=M-1" << endl;
      outputStream << "M=D" << endl;
    }
    if (segment == "static") {
      outputStream << "@" + registerStr << endl;
      outputStream << "D=M" << endl;
      outputStream << "@SP" << endl;
      outputStream << "M=M+1" << endl;
      outputStream << "A=M-1" << endl;
      outputStream << "M=D" << endl;
    }
    if (segment == "temp" || segment == "pointer") {
      outputStream << "@" + registerStr << endl;
      outputStream << "D=A" << endl;
      outputStream << "@" << indexStr << endl;
      outputStream << "A=D+A" << endl;
      outputStream << "D=M" << endl;
      outputStream << "@SP" << endl;
      outputStream << "M=M+1" << endl;
      outputStream << "A=M-1" << endl;
      outputStream << "M=D" << endl;
    }
  }
  void writePop(string segment, int index) {
    string indexStr = to_string(index);
    string registerStr = getRegister(segment, index);

    if (segment == "local" || segment == "argument" || segment == "this" ||
        segment == "that") {
      outputStream << "@" + registerStr << endl;
      outputStream << "D=M" << endl;
      outputStream << "@" + indexStr << endl;
      outputStream << "D=D+A" << endl;
      outputStream << "@SP" << endl;
      outputStream << "M=M-1" << endl;
      outputStream << "A=M+1" << endl;
      outputStream << "M=D" << endl;
      outputStream << "A=A-1" << endl;
      outputStream << "D=M" << endl;
      outputStream << "A=A+1" << endl;
      outputStream << "A=M" << endl;
      outputStream << "M=D" << endl;
    }

    if (segment == "static") {
      outputStream << "@" + registerStr << endl;
      outputStream << "D=A" << endl;
      outputStream << "@SP" << endl;
      outputStream << "M=M-1" << endl;
      outputStream << "A=M+1" << endl;
      outputStream << "M=D" << endl;
      outputStream << "A=A-1" << endl;
      outputStream << "D=M" << endl;
      outputStream << "A=A+1" << endl;
      outputStream << "A=M" << endl;
      outputStream << "M=D" << endl;
    }

    if (segment == "temp" || segment == "pointer") {
      outputStream << "@" + registerStr << endl;
      outputStream << "D=A" << endl;
      outputStream << "@" + indexStr << endl;
      outputStream << "D=D+A" << endl;
      outputStream << "@SP" << endl;
      outputStream << "M=M-1" << endl;
      outputStream << "A=M+1" << endl;
      outputStream << "M=D" << endl;
      outputStream << "A=A-1" << endl;
      outputStream << "D=M" << endl;
      outputStream << "A=A+1" << endl;
      outputStream << "A=M" << endl;
      outputStream << "M=D" << endl;
    }
  }
};
