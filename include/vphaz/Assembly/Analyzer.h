#pragma once

#include <vphaz/config.h>
#include <vphaz/Assembly/Token.h>
#include <vphaz/MachineCode/MachineCode.h>

#include <vector>
#include <map>
#include <string>

BEGIN_NS_ASS

class Analyzer {
public:
    
    MachineCode::Generated analyze(std::vector<Token> const& tokens);
    bool good() const;
    std::string getError() const; 
       
private:

    void segment_code();
    void segment_bss();
    void segment_data();
    
    void add(InstrSet is, InstrTarget t0, InstrTarget t1);
    void add(InstrSet is, InstrTarget t0);
    void add(InstrSet is);

    void getLine();

    Token eat(TokenType type);
    Token eat(TokenType type, std::string const& raw);

    void checkEndInstr();
    void report(std::string const& err);
    std::string error = "";

    MachineCode::Generated code;

    const std::vector<Token>* tokens;
    std::vector<Token> line;
    unsigned int p, pl;
    unsigned int tsize, lsize;
    
    std::map<std::string, unsigned int> label_pos;
    std::map<std::string, unsigned int> data_pos;
    std::map<std::string, unsigned int> bss_pos;

};

END_NS_ASS