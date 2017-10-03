#include <vphaz/Assembly/Analyzer.h>
#include <vphaz/Hardware/CPU/InstrSet.h>
#include <map>
#include <iostream>

BEGIN_NS_ASS

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
/*
        for(auto& t : ts) std::cout << t.raw << ", ";
        std::cout << std::endl;
*/
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

std::vector<Token> Analyzer::getLine() {
    std::vector<Token> ts;
    const unsigned int line = (*tokens)[p].line;
    while(p < tokens_size && line == (*tokens)[p].line) ts.push_back((*tokens)[p++]);
    return ts;
}

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

void Analyzer::segment_bss(MachineCode::Generated& code, std::vector<Token> const& ts) {
    unsigned int s = ts.size();
    if (s != 3) {
        std::cout << "Not enough arguments" << std::endl;
        return;
    } 
    if (ts[0].type != TokenType::Ident) {
        std::cout << "Expected a name" << std::endl;
        return;
    }
    bss_pos.emplace(ts[0].raw, code.bss_size);
    if (ts[1].type != TokenType::Ident) {
        std::cout << "Expected a type" << std::endl;
        return;
    }
    std::string type = ts[1].raw;
    unsigned int size_type;
    if (type == "dw") size_type = 4;
    else if (type == "w") size_type = 2;
    else if (type == "b") size_type = 1;
    else {
        std::cout << "Type unknown" << std::endl;
        return;
    }
    if(ts[2].type == TokenType::Number) {
        int n = std::atoi(ts[2].raw.c_str());
        if (n <= 0) {
            std::cout << "Cannot reserve a negative or null size" << std::endl;
            return;
        }
        code.bss_size += size_type * n;
        std::cout << "# " << size_type << "*" << n << " added to BSS segment" << std::endl;
    } else {
        std::cout << "Expected a number" << std::endl;
        return;
    }
}

void Analyzer::segment_data(MachineCode::Generated& code, std::vector<Token> const& ts) {
    unsigned int s = ts.size();
    if (s <= 2) {
        std::cout << "Arguments missing in data segment" << std::endl;
        return;
    }
    if(ts[0].type != TokenType::Ident) {
        std::cout << "Expected a name" << std::endl;
        return;
    }
    data_pos.emplace(ts[0].raw, code.data.size());
    if (ts[1].type != TokenType::Ident) {
        std::cout << "Expected a type" << std::endl;
        return;
    }
    std::string type = ts[1].raw;
    unsigned int size_type;
    if (type == "dw") size_type = 4;
    else if (type == "w") size_type = 2;
    else if (type == "b") size_type = 1;
    else {
        std::cout << "Type unknown" << std::endl;
        return;
    }

    bool expect_comma = false;
    for (unsigned int i = 2; i < ts.size(); ++i) {
        auto t = ts[i];
        if (expect_comma) {
            if (t.type != TokenType::Comma) {
                std::cout << "Expected a comma" << std::endl;
                return;
            }
            expect_comma = false;
            continue;
        }
        if (t.type == TokenType::Number) {
            int n = std::atoi(t.raw.c_str());
            if (n == 0 && t.raw[0] != '0') {
                std::cout << "Invalid number" << std::endl;
                return;
            }
            for (unsigned int _s = 0; _s < size_type; ++_s) {
                code.data.push_back((n >> (size_type - _s - 1)) & 0xFF);
            }
            std::cout << "# Data added :" << n << std::endl; 
            expect_comma = true;
        } else if (t.type == TokenType::String) {
            for (unsigned int j = 1; j < t.raw.size() - 1; ++j) {
                for (unsigned int _s = 0; _s < size_type - 1 ; ++_s) {
                    code.data.push_back(0);
                }
                char c = t.raw[j];
                code.data.push_back(c);
                std::cout << "# Data added :" << (int)c << " (" << c << ")" << std::endl; 
            }
            expect_comma = true;
        }
    }
    if (!expect_comma) {
        std::cout << "Expected a value" << std::endl;
        return;
    }
}

void Analyzer::add(MachineCode::Generated& code, InstrSet is, InstrTarget t0, InstrTarget t1) {
    code.code.push_back((wordv(is) << 16) | (bytev(t0) << 8) | bytev(t1));    
}

void Analyzer::add(MachineCode::Generated& code, InstrSet is, InstrTarget t0) {
    code.code.push_back((wordv(is) << 16) | (bytev(t0) << 8));
}

void Analyzer::add(MachineCode::Generated& code, InstrSet is) {
    code.code.push_back(wordv(is) << 16);
}

END_NS_ASS