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

    entry_point_label = "";
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

    segment_code();
    if (!good())
        return {};

    return code;
}

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

void Analyzer::segment_code() {
    eat(TokenType::Ident, "segment");
    eat(TokenType::Ident, "code");
    eat(TokenType::Colon);
    checkEndInstr();

    getLine();
    if (!good()) return;

    entry_point_label = "";

    while((p + pl) < tsize && line[pl].raw != "segment") {
        auto t = eat(TokenType::Ident);
        
        if (lsize == 2 && t.type == TokenType::Ident && line[1].type == TokenType::Colon) {
            label_pos.emplace(t.raw, code.code.size());
            std::cout << "# Add label: " << t.raw << std::endl;
            eat(TokenType::Colon);
        
        } else if (lsize == 2 && t.type == TokenType::Ident && t.raw == "entry_point" && line[1].type == TokenType::Ident) {
            entry_point_label = eat(TokenType::Ident).raw;
            std::cout << "# add entry point : " << entry_point_label << std::endl;
        
        } else if (t.raw == "hlt") {
            add(InstrSet::Halt);
            std::cout << "# Add Instruction Halt" << std::endl;
        
        } else if (t.raw == "mov") {
            TargetType ltype, rtype;
            Token lhs = eatTarget(ltype);
            eat(TokenType::Comma);
            Token rhs = eatTarget(rtype);

            switch(ltype) {
            case TargetType::Register: {
                switch(rtype) {
                case TargetType::Register: {
                    add(InstrSet::Move, targetRegisterOf(t0), targetRegisterOf(t1));
                }   
                case TargetType::Addr: {
                    dword addr = findAddressOf(rhs.raw);
                    add(InstrSet::Move, targetRegisterOf(t0), InstrTarget::Addr);
                    add(addr);
                }
                case TargetType::Value: {
                    
                }
                }
            }
            case TargetType::Addr: {
                switch(rtype) {
                    case TargetType::Register: {
                        
                    }   
                    case TargetType::Addr:
                    case TargetType::Value:
                        report("Mov: One of the operant must be a register");
                        break;
                    }
                }
            case TargetType::Value: {
                switch(rtype) {
                    case TargetType::Register: {
                        report("Mov: The l-value must be an address or a register");
                        break;
                    }
                }
            }


                if (rtype != TargetType::Register) { // l is reg, r is value or addr
                    auto it_data = data_pos.find(rhs.raw);
                    bool it_data_valid = (it_data != data_pos.end());

                    auto it_bss = bss_pos.find(rhs.raw);
                    bool it_bss_valid = (it_bss != bss_pos.end());

                    bool is_reg = isRegisterName(rhs.raw);
                    
                    if (!is_reg) {
                        if (!it_bss_valid && !it_data_valid) {
                            report("Can't find this address label");
                        } else if (it_bss_valid && it_data_valid) {
                            report("Name conflict between data and bss adress label");
                        }

                        add(InstrSet::Move, targetRegisterOf(lhs.raw), InstrTarget::Addr);
                        add(it_data_valid ? it_data->second : it_bss->second);
                        std::cout << "# Add Instruction Move reg -> addr" << std::endl;
                    } else {
                        add(InstrSet::Defer, targetRegisterOf(lhs.raw), targetRegisterOf(rhs.raw));
                        std::cout << "# Add Instruction Defer [reg] -> reg" << std::endl;
                    }
                } else { // r is reg or value
                    if (l_is_addr) { // reg -> [reg] or reg -> [addr] 

                    } else { // reg -> reg
                        add(InstrSet::Move, targetRegisterOf(lhs.raw), targetRegisterOf(rhs.raw));
                        std::cout << "# Add Instruction Move reg -> reg" << std::endl;
                    }
                }
            }

        } else 
            report("Unknown command");
        checkEndInstr();
        getLine();
        if (!good()) break;
    }

    if (entry_point_label == "") {
        report("No entry point");
    } else {
        auto it = label_pos.find(entry_point_label);
        if (it == label_pos.end()) {
            report("Cannot find entry_point label");
        } else {
            code.entry_point = it->second;
        }
    }
}

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

        if (isRegisterName(addr.raw))
            return report("this label address name is reserved by a register");
        
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

        if (isRegisterName(addr.raw))
            return report("this label address name is reserved by a register");

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
    code.code.push_back(dwordv(is) << 16);
}

void Analyzer::add(dword dw) {
    code.code.push_back(dw);
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

Token Analyzer::eatRegister() {
    return eat(TokenType::Ident);
}

Token Analyzer::eatAddress() {
    eat(TokenType::LBracket);
    auto t = eat(TokenType::Ident);
    eat(TokenType::RBracket);
    return t;
}

Token Analyzer::eatTarget(TargetType& type) {
    if (line[pl].type == TokenType::LBracket) {
        type = TargetType::Addr;
        return eatAddress();
    } else {
        auto t = eatRegister();
        type = isRegisterName(t) ? TargetType::Register : TargetType::Value;
    }
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

InstrTarget Analyzer::targetRegisterOf(std::string const& reg) {
    if (reg == "eax") return InstrTarget::a32;
    if (reg == "aw") return InstrTarget::a16;
    if (reg == "ah") return InstrTarget::a8lh;
    if (reg == "al") return InstrTarget::a8;

    if (reg == "ebx") return InstrTarget::b32;
    if (reg == "bw") return InstrTarget::b16;
    if (reg == "bh") return InstrTarget::b8lh;
    if (reg == "bl") return InstrTarget::b8;

    if (reg == "ecx") return InstrTarget::c32;
    if (reg == "cw") return InstrTarget::c16;
    if (reg == "ch") return InstrTarget::c8lh;
    if (reg == "cl") return InstrTarget::c8;

    report("Register unknown : " + reg);
    return InstrTarget::a32;
}

bool Analyzer::isRegisterName(std::string const& reg) {
    if (reg == "eax" || reg == "aw" || reg == "ah" || reg == "al") return true;
    if (reg == "ebx" || reg == "bw" || reg == "bh" || reg == "bl") return true;
    if (reg == "ecx" || reg == "cw" || reg == "ch" || reg == "cl") return true;
    return false;
}

dword Analyzer::findAddressOf(std::string const& label) {
    auto it_d = data_pos.find(label);
    auto it_b = bss_pos.find(label);
    if (it_b == bss_pos.end()) {
        if (it_d == data_pos.end()) {
            report("There is no address label called " + label);
            return 0;
        } else {
            return it_d->second;
        }
    } else {
        if (it_d == data_pos.end()) {
            return it_b->second;
        } else {
            report("Conflict between data and bss label");
            return 0;
        }
    }
}

END_NS_ASS