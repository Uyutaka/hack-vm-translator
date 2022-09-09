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
        if (type == CommandType::C_ARITHMETIC) {
          arg1 = line;
        }
        if (type == CommandType::C_POP || type == CommandType::C_PUSH) {
          size_t lastSpace = line.find_last_of(" ");
          if (lastSpace != string::npos) {
            arg1 = line.substr(firstSpace + 1, lastSpace - firstSpace - 1);
            arg2 = atoi(line.substr(lastSpace + 1).c_str());
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
  }
  CommandType getType(const string& type) {
    if (find(ARITHMETIC.begin(), ARITHMETIC.end(), type) != ARITHMETIC.end()) {
      return CommandType::C_ARITHMETIC;
    }
    if (!type.compare("push")) {
      return CommandType::C_PUSH;
    }
    if (!type.compare("pop")) {
      return CommandType::C_POP;
    }
    return CommandType::C_ARITHMETIC;
  }
};
