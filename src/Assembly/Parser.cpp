#include <vphaz/Assembly/Parser.h>
#include <iostream>

BEGIN_NS_ASS

Parser::Parser(std::string const& text) : text(text) {

}

bool Parser::tokenize() {
    tokens.clear();
    p = 0;
    line = 0;

    while(p < text.size()) {
        Token t = getNextToken();
        if (!good()) return false;
        tokens.push_back(t);
    }

    return good();
}

std::string Parser::getError() const {
    return error;
}

bool Parser::good() const {
    return error == "";
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
    if (is_letter(c) || c == '_')
        return getIdent();
    if (c == '"')
        return getString();
        
    error = "Error at " + std::to_string(p) + " '" + std::to_string(c) + "'";
    return Token(TokenType::Unknown, "", line);
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
    bool escape = false;
    while(p < text.size() && (c != '"' || escape)) {
        if (c == '\\') {
            if (escape) {
                s += c;
                escape = false;
            } else {
                escape = true;
            }
            
        } else if (escape) {
            switch(c) {
                case 'n' : s += '\n'; break; 
                case 't' : s += '\t'; break; 
                case 'r' : s += '\r'; break; 
                case 'a' : s += '\a'; break; 
                case '0' : s += '\0'; break;
                case '"' : s += c; break; 
                default:
                    error = "Escape unknown";
                    return Token(TokenType::Unknown, "", line);
            }
            escape = false;
        } else {
            s += c;            
        } 
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

END_NS_ASS