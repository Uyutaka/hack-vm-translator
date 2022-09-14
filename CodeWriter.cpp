#include <iostream>
#include <vector>

#include "Command.h"

using namespace std;
class CodeWriter {
 public:
  CodeWriter(ostream& outputStream) : outputStream(outputStream) {
    writeInit();
  }

  CodeWriter(const vector<Command>& commands, ostream& outputStream,
             const string fileName, bool isDebug = false)
      : commands(commands),
        outputStream(outputStream),
        fileName(fileName),
        isDebug(isDebug) {}
  void writeAsm() {
    for (Command command : commands) {
      if (isDebug) outputStream << "// " << command.vm << endl;
      switch (command.command) {
        // Logical Commands
        case C_ARITHMETIC:
          writeArithmetic(command.arg1);
          break;

        // Memory Access Commands
        case C_POP:
          writePushPop(command);
          break;
        case C_PUSH:
          writePushPop(command);
          break;

        // Branching Commands
        case C_LABEL:
          writeLabel(command.arg1);
          break;
        case C_GOTO:
          writeGoto(command.arg1);
          break;
        case C_IF:
          writeIf(command.arg1);
          break;

        // Function Commands
        case C_CALL:
          writeCall(command.arg1, command.arg2);
          break;
        case C_FUNCTION:
          writeFunction(command.arg1, command.arg2);
          break;
        case C_RETURN:
          writeReturn();
          break;
        default:
          break;
      }
      if (isDebug) outputStream << endl;
    }
  }
  void writeInit() {
    outputStream << "// bootstrap code" << endl;
    string bootstrapLabel = "BOOTSTRAP_" + fileName;

    outputStream << "@256" << endl;
    outputStream << "D=A" << endl;
    outputStream << "@SP" << endl;
    outputStream << "M=D" << endl;
    writeCall("Sys.init", 0);
    outputStream << "(" + bootstrapLabel + ")" << endl;
    outputStream << "@" + bootstrapLabel << endl;
    outputStream << "0;JMP" << endl;
  }

 private:
  vector<Command> commands;
  ostream& outputStream;
  string fileName;
  string functionName;
  int symbolCounter = 0;
  int callCounter = 0;
  bool isDebug = false;
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
    string symbolTrue = "JLT_TRUE_" + to_string(symbolCounter);
    string symbolFalse = "JLT_FALSE_" + to_string(symbolCounter);

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
    if (segment == "static") return fileName + "." + to_string(index);
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
  void writeCall(string functionName, int nArgs) {
    string returnSymbol = functionName + "_RETURN_" + to_string(callCounter);
    callCounter++;

    // push return address
    outputStream << "@" + returnSymbol << endl;
    outputStream << "D=A" << endl;
    outputStream << "@SP" << endl;
    outputStream << "A=M" << endl;
    outputStream << "M=D" << endl;
    outputStream << "@SP" << endl;
    outputStream << "M=M+1" << endl;
    // push LCL
    outputStream << "@LCL" << endl;
    outputStream << "D=M" << endl;
    outputStream << "@SP" << endl;
    outputStream << "A=M" << endl;
    outputStream << "M=D" << endl;
    outputStream << "@SP" << endl;
    outputStream << "M=M+1" << endl;
    // push ARG
    outputStream << "@ARG" << endl;
    outputStream << "D=M" << endl;
    outputStream << "@SP" << endl;
    outputStream << "A=M" << endl;
    outputStream << "M=D" << endl;
    outputStream << "@SP" << endl;
    outputStream << "M=M+1" << endl;
    // push THIS
    outputStream << "@THIS" << endl;
    outputStream << "D=M" << endl;
    outputStream << "@SP" << endl;
    outputStream << "A=M" << endl;
    outputStream << "M=D" << endl;
    outputStream << "@SP" << endl;
    outputStream << "M=M+1" << endl;
    // push TAHT
    outputStream << "@THAT" << endl;
    outputStream << "D=M" << endl;
    outputStream << "@SP" << endl;
    outputStream << "A=M" << endl;
    outputStream << "M=D" << endl;
    outputStream << "@SP" << endl;
    outputStream << "M=M+1" << endl;
    // ARG = SP-n-5
    outputStream << "@" + to_string(nArgs) << endl;
    outputStream << "D=A" << endl;
    outputStream << "@5" << endl;
    outputStream << "D=D+A" << endl;
    outputStream << "@SP" << endl;
    outputStream << "D=M-D" << endl;
    outputStream << "@ARG" << endl;
    outputStream << "M=D" << endl;
    // LCL = SP
    outputStream << "@SP" << endl;
    outputStream << "D=M" << endl;
    outputStream << "@LCL" << endl;
    outputStream << "M=D" << endl;
    // goto functionName
    outputStream << "@" + functionName << endl;
    outputStream << "0;JMP" << endl;
    outputStream << "(" + returnSymbol + ")" << endl;
  }

  void writeFunction(string functionName, int nVars) {
    this->functionName = functionName;
    string loopStartLabel = functionName + "_LOOP_START";
    string loopEndLabel = functionName + "_LOOP_END";

    outputStream << "(" + functionName + ")" << endl;
    outputStream << "@" + to_string(nVars) << endl;
    outputStream << "D=A" << endl;
    outputStream << "@R13" << endl;
    outputStream << "M=D" << endl;
    outputStream << "(" + loopStartLabel + ")" << endl;
    outputStream << "@" + loopEndLabel << endl;
    outputStream << "D;JEQ" << endl;
    outputStream << "@0" << endl;
    outputStream << "D=A" << endl;
    outputStream << "@SP" << endl;
    outputStream << "A=M" << endl;
    outputStream << "M=D" << endl;
    outputStream << "@SP" << endl;
    outputStream << "M=M+1" << endl;
    outputStream << "@R13" << endl;
    outputStream << "MD=M-1" << endl;
    outputStream << "@" + loopStartLabel << endl;
    outputStream << "0;JMP" << endl;
    outputStream << "(" + loopEndLabel + ")" << endl;
  }
  void writeReturn() {
    // FRAME = LCL
    outputStream << "@LCL" << endl;
    outputStream << "D=M" << endl;
    // R13 -> FRAME
    outputStream << "@R13" << endl;
    outputStream << "M=D" << endl;
    // RET = *(FRAME-5)
    outputStream << "@5" << endl;
    outputStream << "A=D-A" << endl;
    outputStream << "D=M" << endl;
    // R14 -> RET
    outputStream << "@R14" << endl;
    outputStream << "M=D" << endl;
    // *ARG = pop()
    outputStream << "@SP" << endl;
    outputStream << "AM=M-1" << endl;
    outputStream << "D=M" << endl;
    outputStream << "@ARG" << endl;
    outputStream << "A=M" << endl;
    outputStream << "M=D" << endl;
    // SP = ARG+1
    outputStream << "D=A" << endl;
    outputStream << "@SP" << endl;
    outputStream << "M=D+1" << endl;
    // THAT = *(FRAME-1)
    outputStream << "@R13" << endl;
    outputStream << "AM=M-1" << endl;
    outputStream << "D=M" << endl;
    outputStream << "@THAT" << endl;
    outputStream << "M=D" << endl;
    // THIS = *(FRAME-2)
    outputStream << "@R13" << endl;
    outputStream << "AM=M-1" << endl;
    outputStream << "D=M" << endl;
    outputStream << "@THIS" << endl;
    outputStream << "M=D" << endl;
    // ARG = *(FRAME-3)
    outputStream << "@R13" << endl;
    outputStream << "AM=M-1" << endl;
    outputStream << "D=M" << endl;
    outputStream << "@ARG" << endl;
    outputStream << "M=D" << endl;
    // LCL = *(FRAME-4)
    outputStream << "@R13" << endl;
    outputStream << "AM=M-1" << endl;
    outputStream << "D=M" << endl;
    outputStream << "@LCL" << endl;
    outputStream << "M=D" << endl;
    // goto RET
    outputStream << "@R14" << endl;
    outputStream << "A=M" << endl;
    outputStream << "0;JMP" << endl;
  }
  void writeLabel(string label) {
    // string labelSymbol = to_string(symbolCounter) + "_" + label;
    // symbolCounter++;
    outputStream << "(" + getSymbol(label) + ")" << endl;
  }
  void writeGoto(string label) {
    // string labelSymbol = to_string(symbolCounter) + "_" + label;
    // symbolCounter++;
    outputStream << "@" + getSymbol(label) << endl;
    outputStream << "0;JMP" << endl;
  }
  void writeIf(string label) {
    // string labelSymbol = to_string(symbolCounter) + "_" + label;
    // symbolCounter++;
    outputStream << "@SP" << endl;
    outputStream << "AM=M-1" << endl;
    outputStream << "D=M" << endl;
    outputStream << "@" + getSymbol(label) << endl;
    outputStream << "D;JNE" << endl;
  }
  string getSymbol(string label) {
    if (functionName.empty()) {
      return fileName + ".asm$" + label;
    } else {
      return functionName + "$" + label;
    }
  }
};
