#include <vphaz/Assembly/Instruction.h>
#include <cassert>
#include <vphaz/Bin.h>

BEGIN_NS_ASS

using namespace assemblyInstruction;

Instruction::Instruction(std::string const& s) : name(name) {

}

Instruction& Instruction::match(Pattern p0) {
    assert(!is_building);
    is_building = true;
    current.p0 = p0;
    current.pattern0_is_value = false;
    current.has_pattern0 = true;
    current.has_pattern1 = false;
    
    return *this;
}

Instruction& Instruction::match(Pattern p0, Pattern p1) {
    match(p0);
    current.has_pattern1 = true;
    current.pattern1_is_value = false;
    current.p1 = p1;
    
    return *this;
}

Instruction& Instruction::match(Pattern p0, int v1) {
    match(p0);
    current.has_pattern1 = true;
    current.pattern1_is_value = true;
    current.v1 = v1;
    
    return *this;
}

Instruction& Instruction::match(int v0) {
    assert(!is_building);
    is_building = true;
    current.has_pattern0 = true;
    current.has_pattern1 = false;
    current.pattern0_is_value = true;
    current.v0 = v0;
    
    return *this;
}

Instruction& Instruction::output(InstrSet is, Target t0) {
    assert(is_building);
    is_building = false;

    current.is = is;
    current.has_target0 = true;
    current.has_target1 = false;
    current.t0 = t0;

    patterns.push_back(current);
    return *this;
}

Instruction& Instruction::output(InstrSet is, Target t0, Target t1) {
    assert(is_building);
    is_building = false;

    current.is = is;
    current.has_target0 = true;
    current.has_target1 = true;
    current.t0 = t0;
    current.t1 = t1;
    current.target0_is_value = false;
    
    patterns.push_back(current);
    return *this;
}

bool Instruction::is_matching(std::vector<Token> const& tokens, std::vector<dword>& res) {
    res.clear();
    if (tokens.empty() || tokens[0].type != TokenType::Ident || name != tokens[0].raw)
        return false;
    for (auto& pat : patterns) {
        if (pat.has_pattern0) {
            if (pat.pattern0_is_value) {

            }
        } else {
            if (tokens.size() == 1) {
                createInstrSet(pat, "", "", res);
                return true;
            }
            continue;
        }
    }
    return false;
}

void Instruction::createInstrSet(ISPattern pat, std::string const& s0, std::string const s1, std::vector<dword>& res) {
    dword is = static_cast<word>(pat.is) << 16;
    if (pat.has_target0) {
        is |= bytev(targetTranslation(pat.t0, )) << 8;
    }
    if (pat.has_target1) {
        is |= bytev(targetTranslation(pat.t1));
    }
}

InstrTarget Instruction::targetTranslation(assemblyInstruction::Target t, std::string const& s) {
    switch(t) {
        case Target::Reg: 
        if (s == "eax") return InstrTarget::a32;
        if (s == "aw") return InstrTarget::a16;
        if (s == "al") return InstrTarget::a8;
        if (s == "ah") return InstrTarget::a8lh;

        if (s == "ebx") return InstrTarget::b32;
        if (s == "bw") return InstrTarget::b16;
        if (s == "bl") return InstrTarget::b8;
        if (s == "bh") return InstrTarget::b8lh;

        if (s == "ecx") return InstrTarget::c32;
        if (s == "cw") return InstrTarget::c16;
        if (s == "cl") return InstrTarget::c8;
        if (s == "ch") return InstrTarget::c8lh;
        std::cout << "Register unknown" << std::endl;
        return InstrTarget::a32;
        case Target::DataBssAddr:
        
    }
}

END_NS