VMTranslator: VMTranslator.cpp Parser.cpp CodeWriter.cpp
	g++ -o VMTranslator -Wall -Wextra -pedantic -std=c++17 VMTranslator.cpp Parser.cpp CodeWriter.cpp -lstdc++fs -Os
