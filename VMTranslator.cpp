#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "CodeWriter.cpp"
#include "Parser.cpp"

using std::cout;
using std::endl;
using std::string;
using std::filesystem::directory_iterator;

int main(int argc, char *argv[]) {
  // string param = "Week8/FunctionCalls/SimpleFunction/SimpleFunction.vm";
  string param = "Week8/FunctionCalls/FibonacciElement";
  string directory;

  bool isDebug = true;
  if (argc == 2) {
    param = argv[1];
  }

  if (param.find(".vm") != string::npos) {
    string vmFile;

    vmFile = param;

    ifstream vmStream(vmFile);
    string line;

    vector<string> fileVector;

    if (vmStream.is_open()) {
      while (vmStream) {
        getline(vmStream, line);
        fileVector.push_back(line);
      }
    } else {
      cout << "Couldn't open file\n";
      return 0;
    }

    string outFileName = vmFile.substr(0, vmFile.size() - 2) + "asm";
    ofstream outputFile(outFileName);
    if (!outputFile.good()) {
      cout << "Couldn't make file" << endl;
      return 0;
    }

    string programName =
        vmFile.substr(0, vmFile.size() - 3);  // trim .extension
    size_t lastSlash = programName.find_last_of("/");
    if (lastSlash != string::npos) {
      programName = programName.substr(lastSlash + 1);
    }

    Parser p(fileVector);
    vector<Command> commands = p.getCommands();
    CodeWriter c(commands, outputFile, programName);
    c.writeAsm();
    outputFile.close();
  } else {
    directory = param;  // TODO change to argv[1]

    // outFile
    string fileName = directory;

    size_t lastSlashP = fileName.find_last_of("/");
    if (lastSlashP != string::npos) {
      fileName = fileName.substr(lastSlashP + 1);
    }
    string outFileName = directory + "/" + fileName + ".asm";
    ofstream outputFile(outFileName);
    CodeWriter c{outputFile};  // Write bootstrap

    // writeINIT
    for (const auto &file : directory_iterator(directory)) {
      string path = file.path().u8string();
      if (path.find(".vm") != string::npos) {
        ifstream vmStream(path);
        string line;

        vector<string> fileVector;
        if (vmStream.is_open()) {
          while (vmStream) {
            getline(vmStream, line);
            fileVector.push_back(line);
          }
        } else {
          cout << "Couldn't open file\n";
          return 0;
        }
        size_t lastSlashP = path.find_last_of("/");
        string programName =
            path.substr(lastSlashP + 1, path.size() - 3);  // trim .extension

        Parser p(fileVector);
        vector<Command> commands = p.getCommands();
        CodeWriter cw(commands, outputFile, programName, true);
        cw.writeAsm();
      }
    }

    outputFile.close();
  }

  return 0;
}
