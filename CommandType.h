#ifndef COMMANDTYPE_H_
#define COMMANDTYPE_H_

enum CommandType {
  C_ARITHMETIC,  // EX: add
  C_PUSH,        // EX: push local 0
  C_POP,         // EX: pop pointer 1
  C_LABEL,       // EX: label xxx
  C_GOTO,        // EX: goto xxx
  C_IF,          // EX: if-goto xxx
  C_FUNCTION,    // EX: function functionName nVars
  C_RETURN,      // EX: return
  C_CALL,        // EX: call functionName nArgs
};

extern enum CommandType commandType;
#endif
