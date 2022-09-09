#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "CodeWriter.cpp"
#include "Parser.cpp"

using namespace std;

int main(int argc, char *argv[]) {
  string vmFile = "test/BasicTest.vm";
  if (2 == argc) {
    vmFile = argv[1];
  }

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

  string programName = vmFile.substr(0, vmFile.size() - 3);  // trim .extension
  size_t lastSlash = programName.find_last_of("/");
  if (lastSlash != string::npos) {
    programName = programName.substr(lastSlash + 1);
  }

  Parser p(fileVector);
  vector<Command> commands = p.getCommands();
  CodeWriter c(commands, outputFile, programName);
  c.writeAsm();
  outputFile.close();

  return 0;
}
