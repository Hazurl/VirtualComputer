#pragma once

#include <vphaz/config.h>
#include <string>

BEGIN_NS_ASS

enum class TokenType {
    Number, String, Ident,
    Colon, LBracket, RBracket, 
    Comma,

    Unknown
};

struct Token {
    TokenType type;
    std::string raw;
    unsigned int line;

    Token() : type(TokenType::Unknown), raw(""), line(0) {}
    Token(TokenType type, std::string const& s, unsigned int l) : type(type), raw(s), line(l) {}
    
};

END_NS_ASS