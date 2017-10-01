#pragma once

#include <vphaz/config.h>
#include <vphaz/Assembly/Token.h>
#include <vphaz/MachineCode/MachineCode.h>

#include <vector>
#include <map>
#include <string>

BEGIN_NS

class Analyzer {
public:
    
    MachineCode::Generated analyze(std::vector<Token> const& tokens);

private:

    void segment_code(MachineCode::Generated& code, std::vector<Token> const& ts);
    void segment_bss(MachineCode::Generated& code, std::vector<Token> const& ts);
    void segment_data(MachineCode::Generated& code, std::vector<Token> const& ts);
    
    void add(MachineCode::Generated& code, InstrSet is, InstrTarget t0, InstrTarget t1);
    void add(MachineCode::Generated& code, InstrSet is, InstrTarget t0);
    void add(MachineCode::Generated& code, InstrSet is);

    std::vector<Token> getLine();

    unsigned int p;
    unsigned int tokens_size;
    const std::vector<Token>* tokens;

    std::string entry_point_label = "";
    
    std::map<std::string, unsigned int> label_pos;
    std::map<std::string, unsigned int> data_pos;
    std::map<std::string, unsigned int> bss_pos;

};

END_NS