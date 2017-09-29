#include <vphaz/Assembly/Parser.h>
#include <iostream>

BEGIN_NS

Parser::Parser(std::string const& text) : text(text) {

}

bool Parser::tokenize() {
    tokens.clear();
    p = 0;
    line = 0;

    while(p < text.size()) {
        Token t = getNextToken();
        if (t.type == TokenType::Unknown)
            return p < text.size();
        tokens.push_back(t);
    }

    return true;
}

std::vector<Token> Parser::getTokens() const {
    return tokens;
}

Token Parser::getNextToken() {
    passBlank();

    if (p >= text.size())
        return Token();
    char c = text[p];

    if (c == ',')
        return (++p, Token(TokenType::Comma, std::string(1, c), line));
    if (c == ':')
        return (++p, Token(TokenType::Colon, std::string(1, c), line));
    if (c == '[')
        return (++p, Token(TokenType::LBracket, std::string(1, c), line));
    if (c == ']')
        return (++p, Token(TokenType::RBracket, std::string(1, c), line));

    if (is_digit(c))
        return getNumber();
    if (is_letter(c) || c == '_' || c == '-')
        return getIdent();
    if (c == '"')
        return getString();
        
    std::cout << "Error at " << p << " '" << c << "'" << std::endl;
    throw 42;
}

bool Parser::is_letter(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Parser::is_digit(char c) const {
    return c >= '0' && c <= '9';
}

Token Parser::getString() {
    std::string s = "";
    s += text[p++];
    char c = text[p];
    while(p < text.size() && c != '"') {
        s += c;
        c = text[++p];
    }
    ++p;
    return Token(TokenType::String, s + c, line);
}

Token Parser::getNumber(){
    std::string s = "";
    s += text[p++];
    char c = text[p];
    while(p < text.size() && is_digit(c)) {
        s += c;
        c = text[++p];
    }
    return Token(TokenType::Number, s, line);
}

Token Parser::getIdent() {
    std::string s = "";
    s += text[p++];
    char c = text[p];
    while(p < text.size() && (is_letter(c) || is_digit(c) || c == '_' || c == '-')) {
        s += c;
        c = text[++p];
    }
    return Token(TokenType::Ident, s, line);
}

void Parser::passBlank() {
    char c = text[p];
    while(c == '\t' || c == '\n' || c == ' ') {
        if (c == '\n') ++line;
        c = text[++p];
    }
}

END_NS