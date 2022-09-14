#include <algorithm>

#include "Command.h";

using namespace std;

class Parser {
 public:
  Parser(const vector<string>& file) {
    file_ = file;

    for (string line : file) {
      clear(line);

      if (!line.empty()) {
        // Extract Command
        CommandType type;
        string arg1 = "";

        int arg2 = 0;
        size_t firstSpace = line.find_first_of(" ");
        if (firstSpace != string::npos) {
          type = getType(line.substr(0, firstSpace));
        } else {
          type = getType(line);
        }
        if (type == C_ARITHMETIC) {
          arg1 = line;
        }
        if (type == C_POP || type == C_PUSH || type == C_FUNCTION ||
            type == C_CALL) {
          size_t lastSpace = line.find_last_of(" ");
          if (lastSpace != string::npos) {
            arg1 = line.substr(firstSpace + 1, lastSpace - firstSpace - 1);
            arg2 = atoi(line.substr(lastSpace + 1).c_str());
          }
        }

        if (type == C_LABEL || type == C_GOTO || type == C_IF) {
          if (firstSpace != string::npos) {
            arg1 = line.substr(firstSpace + 1);
          }
        }

        Command c{type, arg1, arg2, line};
        commands_.push_back(c);
      }
    }
  }
  vector<Command> getCommands() { return commands_; }

 private:
  vector<string> file_;
  vector<Command> commands_;

  const vector<string> ARITHMETIC = {"add", "sub", "neg", "eq", "gt",
                                     "lt",  "and", "or",  "not"};

  void clear(string& line) {
    // Remove \r
    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

    // Remove inline comment
    size_t found = line.find("//");
    if (found != string::npos) {
      line = line.substr(0, found);
    }

    // Remove end-of-line spaces
    size_t endSpace = line.find_last_of(" ");
    if (line.size() != 0) {
      while (line.find_last_of(" ") == line.size() - 1) {
        line = line.substr(0, line.find_last_of(" "));
      }
    }
  }
  CommandType getType(const string& type) {
    if (find(ARITHMETIC.begin(), ARITHMETIC.end(), type) != ARITHMETIC.end()) {
      return C_ARITHMETIC;
    }
    if (!type.compare("push")) {
      return C_PUSH;
    }
    if (!type.compare("pop")) {
      return C_POP;
    }
    if (!type.compare("label")) {
      return C_LABEL;
    }
    if (!type.compare("goto")) {
      return C_GOTO;
    }
    if (!type.compare("if-goto")) {
      return C_IF;
    }
    if (!type.compare("function")) {
      return C_FUNCTION;
    }
    if (!type.compare("call")) {
      return C_CALL;
    }
    if (!type.compare("return")) {
      return C_RETURN;
    }
    return C_ARITHMETIC;
  }
};
