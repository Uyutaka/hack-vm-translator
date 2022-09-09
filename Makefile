all: clean build run test1

build: VMTranslator.cpp Parser.cpp CodeWriter.cpp
	g++ -o VMTranslator -Wall -Wextra -pedantic -std=c++17 VMTranslator.cpp Parser.cpp CodeWriter.cpp -lstdc++fs -Os

run:
	./VMTranslator test/BasicTest.vm
	./VMTranslator test/PointerTest.vm
	./VMTranslator test/SimpleAdd.vm
	./VMTranslator test/StackTest.vm
	./VMTranslator test/StaticTest.vm

clean:
	rm VMTranslator

	rm test/BasicTest.asm
	rm test/PointerTest.asm
	rm test/SimpleAdd.asm
	rm test/StackTest.asm
	rm test/StaticTest.asm

test1:
	diff test/BasicTest.asm test/BasicTest.expected_asm
	diff test/PointerTest.asm test/PointerTest.expected_asm
	diff test/SimpleAdd.asm test/SimpleAdd.expected_asm
	diff test/StackTest.asm test/StackTest.expected_asm
	diff test/StaticTest.asm test/StaticTest.expected_asm
