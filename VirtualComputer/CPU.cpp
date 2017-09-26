#include "stdafx.h"
#include "CPU.h"
#include <iostream>
#include "ALUInstrSetUnknown.h"
#include "InstrSetUnknown.h"
#include "WrongUtilisationRam.h"

BEGIN_NS

CPU::CPU(Bus32* bus_data, Bus32* bus_addr, MemoryControlFlow32* ram, Processable* out) :
	bus_data(bus_data),
	reg_flag(&bus_ALUflag, &bus_CUflag),
	reg_memory(bus_data, bus_addr),
	reg_a(bus_data, bus_data),
	reg_b(bus_data, bus_data),
	reg_c(bus_data, bus_data),
	reg_instr(bus_data, &bus_instr),
	reg_instrAddr(bus_data, bus_data),
	reg_ALUTmp(bus_data, &bus_ALUTmp),
	reg_ALURes(&bus_ALURes, bus_data),
	reg_stack_ptr(bus_data, bus_data),
	reg_seg_instr(bus_data, bus_data),
	reg_seg_stack(bus_data, bus_data),
	reg_seg_bss(bus_data, bus_data),
	reg_seg_data(bus_data, bus_data),
	alu(&bus_ALUCode, &bus_ALUflag, &bus_ALURes, bus_data, &bus_ALUTmp),
	cu(ram, &alu, out, &reg_memory, &reg_flag, &bus_CUflag, &reg_instr, &bus_instr, &reg_instrAddr, &bus_ALUCode, 
	   &reg_ALURes, &reg_ALUTmp, &reg_a, &reg_b, &reg_c, &reg_seg_instr, &reg_seg_stack, &reg_stack_ptr, &reg_seg_data, &reg_seg_bss)
{

}

void CPU::define_program(udword segment_instr, udword segment_data, udword segment_bss, udword segment_stack, udword entry_point) {
	bus_data->bind_32(segment_instr);
	reg_seg_instr.read32();

	bus_data->bind_32(segment_data);
	reg_seg_data.read32();

	bus_data->bind_32(segment_bss);
	reg_seg_bss.read32();

	bus_data->bind_32(segment_stack);
	reg_seg_stack.read32();

	bus_data->bind_32(entry_point);
	reg_instrAddr.read32();
}

void CPU::start_program() {
	try {
		cu.halted = false;
		while(!cu.halted)
			cu.process();

	} catch(InstrSetUnknown const& i) {
		std::cout << "InstrSetUnknown: " << std::endl;
		std::cout << i.what() << std::endl;
	} catch(WrongRamUtilisation const& w) {
		std::cout << "WrongRamUtilisation: " << std::endl;
		std::cout << w.what() << std::endl;
	} catch(ALUInstrSetUnknown const& i) {
		std::cout << "ALUInstrSetUnknown: " << std::endl;
		std::cout << i.what() << std::endl;
	} catch(...) {
		std::cout << "Exception unknown!" << std::endl;
	}
}

END_NS