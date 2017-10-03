#include <vphaz/Assembly/Analyzer.h>
#include <vphaz/Hardware/CPU/InstrSet.h>
#include <map>
#include <iostream>

BEGIN_NS_ASS

MachineCode::Generated Analyzer::analyze(std::vector<Token> const& tokens) {
    this->tokens = &tokens;
    tsize = tokens.size();
    p = 0;
    code = MachineCode::Generated();

    label_pos.clear();
    data_pos.clear();
    bss_pos.clear();
    
    getLine();
    if (!good())
        return {};

    segment_bss();
    if (!good())
        return {};

    segment_data();
    if (!good())
        return {};
/*
    segment_code();
    if (!good())
        return {};
*/
    return code;
}
/*
MachineCode::Generated Analyzer::analyze(std::vector<Token> const& tokens) {
    MachineCode::Generated code;

    p = 0;
    this->tokens = &tokens;
    tokens_size = tokens.size();

    const int SEG_CODE = 1;
    const int SEG_DATA = 2;
    const int SEG_BSS = 3;

    int current_seg = 0;
    bool has_seg_code = false;
    bool has_seg_data = false;
    bool has_seg_bss = false;

    while(p < tokens_size) {
        auto ts = getLine();
        unsigned int s = ts.size();

        for(auto& t : ts) std::cout << t.raw << ", ";
        std::cout << std::endl;

        // segment entry "segment code:"
        if (s == 3 && ts[0].type == TokenType::Ident && ts[0].raw == "segment" &&
            ts[1].type == TokenType::Ident &&
            ts[2].type == TokenType::Colon) {

            switch(current_seg) {
            case SEG_BSS:
                has_seg_bss = true;
                break;
            case SEG_DATA:
                has_seg_data = true;
                break;
            case SEG_CODE:
                has_seg_code = true;
                break;
            default: break;
            }
                
            auto seg = ts[1].raw;
            if (seg == "data") {
                if (has_seg_data) {
                    std::cout << "Data segment already defined" << std::endl;
                    return code;
                } else {
                    current_seg = SEG_DATA;
                    std::cout << std::endl << "# Start Segment Data" << std::endl;
                }
            } else if (seg == "code") {
                if (has_seg_code) {
                    std::cout << "Code segment already defined" << std::endl;
                    return code;
                } else {
                    current_seg = SEG_CODE;
                    std::cout << std::endl << "# Start Segment Code" << std::endl;
                }
            } else if (seg == "bss") {
                if (has_seg_bss) {
                    std::cout << "BSS segment already defined" << std::endl;
                    return code;
                } else {
                    current_seg = SEG_BSS;
                    std::cout << std::endl << "# Start Segment BSS" << std::endl;
                }
            } else {
                std::cout << "Segment unknown" << std::endl;
                return code;
            }
            continue;
        }

        if (current_seg == SEG_CODE) {
            segment_code(code, ts);
        }

        if (current_seg == SEG_BSS) {
            segment_bss(code, ts);
        }

        if (current_seg == SEG_DATA) {
            segment_data(code, ts);
        }
    }
    std::cout << std::endl;

    if (entry_point_label == "") {
        std::cout << "Link error, can't find an entry_point" << std::endl;
    } else {
        auto it = label_pos.find(entry_point_label);
        if (it == label_pos.end()) {
            std::cout << "Link error, this label doesn't exists \"" << entry_point_label << "\"" << std::endl;
        } else {
            std::cout << "# Entry point: " << it->second << std::endl;
            code.entry_point = it->second;
        }
    }

    std::cout << "Data: ";
    for (auto d : code.data) {
        std::cout << (int)d << " (" << d << ") ";
    }
    std::cout << std::endl;

    std::cout << "BSS: " << code.bss_size << " bytes (" << code.bss_size * 8 << " bits)" << std::endl;
    return code;
}
*/
void Analyzer::getLine() {
    const unsigned int l = (*tokens)[p].line;
    line.clear();
    while(p < tsize && l == (*tokens)[p].line) line.push_back((*tokens)[p++]);
    pl = 0;
    lsize = line.size();
/*
    std::cout << "LINE {";
    for (auto& t : line) std::cout << t.raw << ", ";
    std::cout << "}" << std::endl;
*/
}
/*
void Analyzer::segment_code(MachineCode::Generated& code, std::vector<Token> const& ts) {
    unsigned int s = ts.size();
    // label
    if (s == 2 && ts[0].type == TokenType::Ident && ts[1].type == TokenType::Colon) {
        label_pos.emplace(ts[0].raw, code.code.size());
        std::cout << "# Add label: " << ts[0].raw << std::endl;
        return;
    }

    // entry_point "entry_point mylabel"
    if (entry_point_label == "" && s == 2 && ts[0].type == TokenType::Ident && ts[0].raw == "entry_point" &&
        ts[1].type == TokenType::Ident) {
        entry_point_label = ts[1].raw;
        return;
    }

    if (ts[0].type == TokenType::Ident) {
        std::string cmd = ts[0].raw;
        if (cmd == "mov") {
            if(s < 4) {
                std::cout << "Not enough arguments" << std::endl;
                return;
            }
            std::string a, b;
            bool addr0 = false, addr1 = false;
            if (ts[1].type == TokenType::LBracket) {
                addr0 = true;
                if (s < 5 || ts[2].type != TokenType::Ident || ts[3].type != TokenType::RBracket || ts[4].type != TokenType::Comma) {
                    std::cout << "Can't resolve mov command" << std::endl;
                    return;
                }
                a = ts[2].raw;
                if (ts[5].type == TokenType::LBracket) {
                    addr1 = true;
                    if (s != 8 || ts[6].type != TokenType::Ident || ts[7].type != TokenType::RBracket) {
                        std::cout << "Can't resolve mov command" << std::endl;
                        return;
                    }
                    b = ts[6].raw;
                } else {
                    if (s != 6 || ts[5].type != TokenType::Ident) {
                        std::cout << "Can't resolve mov command" << std::endl;
                        return;
                    }
                    b = ts[5].raw;
                }   
            } else {
                if (s < 4 || ts[1].type != TokenType::Ident || ts[2].type != TokenType::Comma) {
                    std::cout << "Can't resolve mov command" << std::endl;
                    return;
                }
                a = ts[1].raw;
                if (ts[3].type == TokenType::LBracket) {
                    addr1 = true;
                    if (s != 6 || ts[4].type != TokenType::Ident || ts[5].type != TokenType::RBracket) {
                        std::cout << "Can't resolve mov command" << std::endl;
                        return;
                    }
                    b = ts[5].raw;
                } else {
                    if (s != 4 || ts[3].type != TokenType::Ident) {
                        std::cout << "Can't resolve mov command" << std::endl;
                        return;
                    }
                    b = ts[3].raw;
                }   
            }

            

        } else if (cmd == "hlt") {
            if (s != 1) {
                std::cout << "Halt don't need any arguments" << std::endl;
                return;
            }
            add(code, InstrSet::Halt);
            std::cout << "# Add Halt command" << std::endl;
            return;
        }  else if (cmd == "add") {

        } else if (cmd == "cmp") {

        } else if (cmd == "mul") {

        } else if (cmd == "div") {

        } else if (cmd == "sub") {

        } else if (cmd == "jmp") {

        } else if (cmd == "jz") {

        } else if (cmd == "jl") {

        } else if (cmd == "jg") {

        } else if (cmd == "jge") {

        } else if (cmd == "jle") {

        } else if (cmd == "out") {

        } else if (cmd == "inc") {

        }
    }
}
*/
void Analyzer::segment_bss() {
    eat(TokenType::Ident, "segment");
    eat(TokenType::Ident, "bss");
    eat(TokenType::Colon);
    checkEndInstr();

    getLine();
    if (!good()) return;

    while(p < tsize && line[pl].raw != "segment") {
        auto addr = eat(TokenType::Ident);
        auto type = eat(TokenType::Ident);
        auto size_request = eat(TokenType::Number);

        checkEndInstr();
        getLine();
        if (!good()) return;
        
        bss_pos.emplace(addr.raw, code.bss_size);

        std::string rawtype = type.raw;

        unsigned int size_type;
        if (rawtype == "dw") size_type = 4;
        else if (rawtype == "w") size_type = 2;
        else if (rawtype == "b") size_type = 1;
        else return report("Type unknown");

        int n = std::atoi(size_request.raw.c_str()) * size_type;
        if (n <= 0) report("Cannot reserve a negative size of byte");
        code.bss_size += n;

        std::cout << "# " << size_type << "*" << n / size_type << " (" << n * 8 << " bytes) added to BSS segment" << std::endl;
        if (!good()) return;
    }
}

void Analyzer::segment_data() {
    eat(TokenType::Ident, "segment");
    eat(TokenType::Ident, "data");
    eat(TokenType::Colon);
    checkEndInstr();

    getLine();
    if (!good()) return;

    while(p < tsize && line[pl].raw != "segment") {
        auto addr = eat(TokenType::Ident);
        auto type = eat(TokenType::Ident);

        data_pos.emplace(addr.raw, code.data.size());
        std::string rawtype = type.raw;
        unsigned int size_type;
        if (rawtype == "dw") size_type = 4;
        else if (rawtype == "w") size_type = 2;
        else if (rawtype == "b") size_type = 1;
        else return report("Type unknown");

        bool expect_value = true;
        while (pl < lsize) {
            auto t = line[pl++];
            if (!expect_value) {
                if (t.type != TokenType::Comma)
                    report("Expected a comma");
                expect_value = true;

            } else if (t.type == TokenType::Number) {
                int n = std::atoi(t.raw.c_str());
                if (n == 0 && t.raw[0] != '0')
                    report("Invalid Value");

                // set each byte
                for (unsigned int _s = 0; _s < size_type; ++_s)
                    code.data.push_back((n >> (size_type - _s - 1)) & 0xFF);

                std::cout << "# Data added :" << n << std::endl; 
                expect_value = false;

            } else if (t.type == TokenType::String) {
                for (unsigned int j = 1; j < t.raw.size() - 1; ++j) {
                    // char are only 1 byte, but user can change it to more, so we fill with 0
                    for (unsigned int _s = 0; _s < size_type - 1 ; ++_s)
                        code.data.push_back(0);

                    char c = t.raw[j];
                    code.data.push_back(c);
                    std::cout << "# Data added :" << (int)c << " (" << c << ")" << std::endl; 
                }
                expect_value = false;
            }
        }
        if (expect_value)
            report("Expected a value");

        checkEndInstr();
        getLine();
        if (!good()) return;
    }
}

void Analyzer::add(InstrSet is, InstrTarget t0, InstrTarget t1) {
    code.code.push_back((wordv(is) << 16) | (bytev(t0) << 8) | bytev(t1));    
}

void Analyzer::add(InstrSet is, InstrTarget t0) {
    code.code.push_back((wordv(is) << 16) | (bytev(t0) << 8));
}

void Analyzer::add(InstrSet is) {
    code.code.push_back(wordv(is) << 16);
}

Token Analyzer::eat(TokenType type) {
    if (pl >= lsize) {
        report("Expected Type::" + std::to_string(static_cast<int>(type)) + " and got nothing");
        return Token(type, "", 0);
    }
    Token t = line[pl];
    if (t.type != type) {
        report("Expected : Type::" + std::to_string(static_cast<int>(type)) + " and got Type::" + std::to_string(static_cast<int>(t.type)));        
    }
//    std::cout << "Eat " << t.raw << std::endl;
    pl++;
    return t;
}

Token Analyzer::eat(TokenType type, std::string const& raw) {
    auto t = eat(type);
    if (t.raw != raw) {
        report("Expected : " + raw + " and got " + t.raw);        
    }
//    std::cout << "Eat " << raw << std::endl;
    return t;
}

bool Analyzer::good() const {
    return error == "";
}

std::string Analyzer::getError() const {
    return error;
}

void Analyzer::checkEndInstr() {
    if (pl < lsize)
        report("Expected end of instruction");
}

void Analyzer::report(std::string const& err) {
    if (good())
        error = err;
    else
        error += "; " + err;
}

END_NS_ASS