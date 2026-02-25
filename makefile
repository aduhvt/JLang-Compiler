main: main.o lexer/lexer.o parser/parser.o
	g++ main.o lexer/lexer.o parser/parser.o -o main

main.o: main.cpp lexer/lexer.h parser/parser.h
	g++ -c main.cpp

lexer/lexer.o: lexer/lexer.cpp lexer/lexer.h
	g++ -c lexer/lexer.cpp -o lexer/lexer.o

parser/parser.o: parser/parser.cpp parser/parser.h
	g++ -c parser/parser.cpp -o  parser/parser.o

clean: 
	del main.exe main.o lexer\lexer.o parser\parser.o