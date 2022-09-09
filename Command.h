
#ifndef COMMAND_H_
#define COMMAND_H_
#include <string>;

#include "CommandType.h";
using namespace std;
struct Command {
  CommandType command;
  string arg1;
  int arg2;
  string vm;
};

extern struct Command command;
#endif
