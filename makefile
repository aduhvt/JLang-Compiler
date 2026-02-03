main: main.o lexer/lexer.o
	g++ main.o lexer/lexer.o -o main

main.o: main.cpp lexer/lexer.h
	g++ -c main.cpp

lexer/lexer.o: lexer/lexer.cpp lexer/lexer.h
	g++ -c lexer/lexer.cpp -o lexer/lexer.o

clean: 
	del main.exe main.o lexer\lexer.o