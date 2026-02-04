#ifndef LEXER_LEXER_H
#define LEXER_LEXER_H

#include <vector>
#include <string>

    enum class TokenType{
        // Keywords
        KW_INT,
        KW_BOOL,
        KW_PRINT,
        KW_IF,
        KW_ELSE,
        KW_LOOP,
        KW_BREAK,

        // Identifier & Number
        IDENTIFIER,
        NUMBER,

        // Operators
        PLUS,
        MINUS,
        STAR,
        SLASH,
        EQUAL,
        EQUAL_EQUAL,
        BANG_EQUAL,
        LESS,
        GREATER,

        // Punctuation
        LEFT_PAREN,
        RIGHT_PAREN,
        LEFT_BRACE,
        RIGHT_BRACE,
        SEMICOLON,

        // Special
        END_OF_FILE,
        ERROR
    };

    struct Token{
        TokenType type;
        std::string lexeme;
        int line;
    };

    class lexer{
        private:
            std::vector<Token> tokens;

        public:
            void tokenize(std::vector<std::string> &lines);
            const std::vector<Token>& getTokens() const; // this function has lore
    };

#endif