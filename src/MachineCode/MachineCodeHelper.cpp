#include <vphaz/MachineCode/MachineCodeHelper.h>

BEGIN_NS

MachineCode MachineCodeHelper::generate_fibonacci() {
	MachineCode m;
	// 0 bits bss
	m.add_data_32( 1 ); // 32 bits : fibo. init
	m.add_data_32( 45 ); // 32 bits : n (from fibo. seq)
	
	m.command(InstrSet::Move, InstrTarget::a32, InstrTarget::Addr);
	m.command(0);
	m.command(InstrSet::Move, InstrTarget::b32, InstrTarget::Addr);
	m.command(0);
	m.command(InstrSet::Move, InstrTarget::c32, InstrTarget::Addr);
	m.command(4);

	m.label(0);
	m.command(InstrSet::Comp0, InstrTarget::c32);
	m.command(InstrSet::Jmplz, InstrTarget::Value);
	m.go_to(1);

	m.command(InstrSet::Out, InstrTarget::a32);
	m.command(InstrSet::Push, InstrTarget::a32);
	m.command(InstrSet::Add, InstrTarget::a32, InstrTarget::b32);
	m.command(InstrSet::Pop, InstrTarget::b32);
	m.command(InstrSet::Dec, InstrTarget::c32);

	m.command(InstrSet::Jmp, InstrTarget::Value);
	m.go_to(0);

	m.label(1);
	m.command(InstrSet::Out, InstrTarget::a32);
	m.command(InstrSet::Halt);

	m.request_bss(0);
	m.request_stack(128);

	return m;
}

MachineCode MachineCodeHelper::generate_counter() {
	MachineCode m;
	// 0 bits bss
	// 0 bits data

	m.command(InstrSet::Move, InstrTarget::a32, InstrTarget::Value);
	m.command(10);
	m.label(0);
	m.command(InstrSet::Out, InstrTarget::a32);
	m.command(InstrSet::Dec, InstrTarget::a32);
	m.command(InstrSet::Comp0, InstrTarget::a32);
	m.command(InstrSet::Jmpnz, InstrTarget::Value);
	m.go_to(0);
	m.command(InstrSet::Out, InstrTarget::a32);
	m.command(InstrSet::Halt);

	m.request_bss(0);
	m.request_stack(0);

	return m;
}

MachineCode MachineCodeHelper::generate_simple_counter() {
	MachineCode m;
	// 0 bits bss
	// 0 bits data

	m.command(InstrSet::Move, InstrTarget::a32, InstrTarget::Value);
	m.command(0);
	m.label(0);
	m.command(InstrSet::Out, InstrTarget::a32);
	m.command(InstrSet::Dec, InstrTarget::a32);

	m.command(InstrSet::Out, InstrTarget::a32);
	m.command(InstrSet::Dec, InstrTarget::a32);

	m.command(InstrSet::Out, InstrTarget::a32);
	m.command(InstrSet::Dec, InstrTarget::a32);

	m.command(InstrSet::Out, InstrTarget::a32);
	m.command(InstrSet::Dec, InstrTarget::a32);

	m.command(InstrSet::Out, InstrTarget::a32);
	m.command(InstrSet::Halt);

	m.request_bss(0);
	m.request_stack(0);

	return m;
}

MachineCode MachineCodeHelper::generate_for_in_range() {
	MachineCode m;
	// 0 bits bss
	// 0 bits data

	m.command(InstrSet::Move, InstrTarget::a32, InstrTarget::Value);
	m.command(125);
	m.command(InstrSet::Move, InstrTarget::b32, InstrTarget::Value);
	m.command(130);
	m.label(0);
	m.command(InstrSet::Out, InstrTarget::a32);
	m.command(InstrSet::Inc, InstrTarget::a32);
	m.command(InstrSet::Comp, InstrTarget::a32, InstrTarget::b32);
	m.command(InstrSet::Jmpl, InstrTarget::Value);
	m.go_to(0);
	m.command(InstrSet::Out, InstrTarget::a32);
	m.command(InstrSet::Halt);

	m.request_bss(0);
	m.request_stack(0);

	return m;
}

MachineCode MachineCodeHelper::generate_hello_world() {
	MachineCode m;
	// 0 bits bss

	m.add_data_8('h');
	m.add_data_8('e');
	m.add_data_8('l');
	m.add_data_8('l');
	m.add_data_8('o');
	m.add_data_8(' ');
	m.add_data_8('w');
	m.add_data_8('o');
	m.add_data_8('r');
	m.add_data_8('l');
	m.add_data_8('d');
	m.add_data_8(' ');
	m.add_data_8('!');
	m.add_data_8(0);

	m.command(InstrSet::Move, InstrTarget::b32, InstrTarget::Value);
	m.command(0);
	
	m.label(1);
	m.command(InstrSet::Defer, InstrTarget::b32, InstrTarget::a8);
	m.command(InstrSet::Comp0, InstrTarget::a8);
	m.command(InstrSet::Jmpz, InstrTarget::Value);
	
	m.go_to(0);
	
	m.command(InstrSet::Out, InstrTarget::a8);
	m.command(InstrSet::Inc, InstrTarget::b32);
	m.command(InstrSet::Jmp, InstrTarget::Value);
	m.go_to(1);

	m.label(0);
	m.command(InstrSet::Halt);

	m.request_bss(0);
	m.request_stack(0);

	return m;
}

END_NS
