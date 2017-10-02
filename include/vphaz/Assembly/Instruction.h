#pragma once

#include <string>
#include <vector>

#include <vphaz/config.h>
#include <vphaz/Assembly/Token.h>
#include <vphaz/Hardware/CPU/InstrSet.h>

BEGIN_NS
/*
Instruction mov_is("mov")
    .match(Pattern::Reg, Pattern::DataBssAddr)  .output(InstrSet::Load, Target::DataBssAddr)
    .match(Pattern::DataAddr, Pattern::Reg)     .output(InstrSet::Store, Target::Reg, Target::DataAddr)
    .match(Pattern::Reg, Patter::Reg)           .output(InstrSet::Load, Target::Reg, Target::Reg);

Instruction hlt_is("hlt")
    .match()                                    .output(InstrSet::Halt);

Instruction cmp_is("cmp")
    .match(Pattern::Reg)                        .output(InstrSet::Comp0, Target::Reg)
    .match(Pattern::Reg, 0)                     .output(InstrSet::Comp0, Target::Reg)
    .match(Pattern::Reg, Pattern::DataBssAddr)  .output(InstrSet::Comp, Target::Reg, Target::DataBssAddr)
    .match(Pattern::Reg, Pattern::Reg)          .output(InstrSet::Comp, Target::Reg, Target::Reg);
*/
namespace assemblyInstruction {
enum class Pattern {
    Reg, 
    DataBssAddr,
    BssAddr,
    DataAddr,
    CodeAddr,
    Value
};
enum class Target {
    Reg, DataBssAddr,
    BssAddr,
    DataAddr,
    CodeAddr,
    Value,
};
}

class Instruction {
public:

    Instruction(std::string const& name);

    Instruction& match(assemblyInstruction::Pattern p0);
    Instruction& match(assemblyInstruction::Pattern p0, assemblyInstruction::Pattern p1);
    Instruction& match(int v0);
    Instruction& match(assemblyInstruction::Pattern p0, int v1);

    Instruction& output(InstrSet is, assemblyInstruction::Target t0);
    Instruction& output(InstrSet is, assemblyInstruction::Target t0, assemblyInstruction::Target t1);
    Instruction& output(InstrSet is, int v0);
    Instruction& output(InstrSet is, assemblyInstruction::Target t0, int v1);
    
    bool is_matching(std::vector<Token> const& tokens, std::vector<dword>& res);

private:
    struct ISPattern {
        bool has_pattern0, has_pattern1;
        bool has_target0, has_target1;
        bool pattern0_is_value, pattern1_is_value;
        union { assemblyInstruction::Pattern p0; int v0; };
        union { assemblyInstruction::Pattern p1; int v1; };
        assemblyInstruction::Target t0;
        assemblyInstruction::Target t1;
        InstrSet is;
    };

    void createInstrSet(ISPattern pat, std::string const& s0, std::string const s1, std::vector<dword>& res);
    InstrTarget targetTranslation(assemblyInstruction::Target t, std::string const& s);


    std::vector<ISPattern> patterns;
    bool is_building = false;
    ISPattern current;

    std::string name;

};

END_NS