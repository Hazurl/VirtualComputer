#include "stdafx.h"
#include "MachineCodeHelper.h"

BEGIN_NS

MachineCode MachineCodeHelper::generate_fibonacci() {
	MachineCode m;
	m.command(InstrSet::Load_a);
	m.command(1);
	m.command(InstrSet::Load_b);
	m.command(1);
	m.command(InstrSet::Load_c);
	m.link(0);

	m.label(0);
	m.command(InstrSet::Comp0_c);
	m.command(InstrSet::Jmplz);
	m.go_to(1);

	m.command(InstrSet::Out_a);
	m.command(InstrSet::Push_a);
	m.command(InstrSet::Add_ab);
	m.command(InstrSet::Pop_b);
	m.command(InstrSet::Dec_c);

	m.command(InstrSet::Jmp);
	m.go_to(0);

	m.label(1);
	m.command(InstrSet::Out_a);
	m.command(InstrSet::Halt);
	return m;
}

MachineCode MachineCodeHelper::generate_counter() {
	MachineCode m;

	m.command(InstrSet::Load_a);
	m.link(0);
	m.label(0);
	m.command(InstrSet::Out_a);
	m.command(InstrSet::Dec_a);
	m.command(InstrSet::Comp0_a);
	m.command(InstrSet::Jmpnz);
	m.go_to(0);
	m.command(InstrSet::Out_a);
	m.command(InstrSet::Halt);

	return m;
}

MachineCode MachineCodeHelper::generate_simple_counter() {
	MachineCode m;

	m.command(InstrSet::Load_a);
	m.link(0);
	m.label(0);
	m.command(InstrSet::Out_a);
	m.command(InstrSet::Dec_a);
	m.command(InstrSet::Out_a);
	m.command(InstrSet::Dec_a);
	m.command(InstrSet::Out_a);
	m.command(InstrSet::Dec_a);
	m.command(InstrSet::Out_a);
	m.command(InstrSet::Dec_a);
	m.command(InstrSet::Out_a);
	m.command(InstrSet::Halt);

	return m;
}

MachineCode MachineCodeHelper::generate_for_in_range() {
	MachineCode m;

	m.command(InstrSet::Load_a);
	m.link(0);
	m.command(InstrSet::Load_b);
	m.link(1);
	m.label(0);
	m.command(InstrSet::Out_a);
	m.command(InstrSet::Inc_a);
	m.command(InstrSet::Comp_ab);
	m.command(InstrSet::Jmpl);
	m.go_to(0);
	m.command(InstrSet::Out_a);
	m.command(InstrSet::Halt);

	return m;
}

END_NS