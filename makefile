main: main.o lexer/lexer.o parser/parser.o semantic/semantic.o interpreter/interpreter.o
	g++ main.o lexer/lexer.o parser/parser.o semantic/semantic.o interpreter/interpreter.o -o main

main.o: main.cpp lexer/lexer.h parser/parser.h semantic/semantic.h interpreter/interpreter.h
	g++ -c main.cpp

lexer/lexer.o: lexer/lexer.cpp lexer/lexer.h
	g++ -c lexer/lexer.cpp -o lexer/lexer.o

parser/parser.o: parser/parser.cpp parser/parser.h
	g++ -c parser/parser.cpp -o  parser/parser.o

semantic/semantic.o : semantic/semantic.cpp semantic/semantic.h
	g++ -c semantic/semantic.cpp -o semantic/semantic.o

interpreter/interpreter.o : interpreter/interpreter.cpp interpreter/interpreter.h
	g++ -c interpreter/interpreter.cpp -o interpreter/interpreter.o

clean: 
	del main.exe main.o lexer\lexer.o parser\parser.o semantic\semantic.o interpreter\interpreter.o