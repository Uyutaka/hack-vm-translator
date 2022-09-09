#ifndef COMMANDTYPE_H_
#define COMMANDTYPE_H_

enum CommandType {
  C_ARITHMETIC,  // EX: add
  C_PUSH,        // EX: push local 0
  C_POP,         // EX: pop pointer 1
};

extern enum CommandType commandType;
#endif
