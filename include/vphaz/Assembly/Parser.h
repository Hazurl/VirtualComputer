#pragma once

#include <vphaz/config.h>
#include <string>
#include <vector>
#include <vphaz/Assembly/Token.h>

BEGIN_NS

class Parser {
public:
    Parser(std::string const& text);

    bool tokenize();

    std::vector<Token> getTokens() const;

private:

    Token getNextToken();

    void passBlank();

    Token getString();
    Token getNumber();
    Token getIdent();

    bool is_letter(char c) const;
    bool is_digit(char c) const;

    std::string text;
    std::vector<Token> tokens;
    unsigned int p;
    unsigned int line;
};

END_NS