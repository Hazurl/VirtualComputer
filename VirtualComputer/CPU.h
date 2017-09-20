#pragma once

#include "ALU.h"
#include "Bus.h"
#include "CU.h"
#include "Register.h"

BEGIN_NS

class CPU {
public:
	CPU(Bus* bus_data);


private:

	Bus* bus_data;

	Bus bus_instr;
	Bus bus_ALURes;
	Bus bus_ALUTmp;
	Bus bus_ALUCode;
	Bus bus_ALUFlag;
	Bus bus_CUFlag;

	Register reg_a;
	Register reg_b;
	Register reg_c;
	Register reg_instr;
	Register reg_instrAddr;
	Register reg_ALUTmp;
	Register reg_ALURes;
	Register reg_flag;

	ALU alu;
	CU cu;
};

END_NS