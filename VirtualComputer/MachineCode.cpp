#include "stdafx.h"
#include "MachineCode.h"
#include "Bin.h"

BEGIN_NS

MachineCode::Instr::Instr(Instr:: State s, dword b) : state(s), cmd(b) {

}

MachineCode::Generated MachineCode::generate() {
	MachineCode::Generated g;
	std::unordered_map<byte, byte> labels; // id -> mem
	std::unordered_map<byte, byte> go_tos; // id -> mem
	for(auto& i : code) {
		switch(i.state) {
		case Instr::State::cmd:
			g.code.emplace_back(i.cmd);
			break;
		case Instr::State::label:
			labels.emplace(i.cmd, (g.code.size() - 1) * sizeof(dword));
			break;
		case Instr::State::label_addr:
			auto it = labels.find(i.cmd);
			if(it != labels.end())
				g.code.emplace_back(it->second + sizeof(dword));
			else {
				go_tos.emplace(i.cmd, g.code.size());
				g.code.emplace_back(0);
			}
			break;
		}
	}

	for(auto& p : go_tos) {
		auto it = labels.find(p.first);
		if(it == labels.end())
			return {};
		g.code[p.second] = it->second;
	}

	g.stack_size = wanted_stack_size;
	g.bss_size = wanted_bss_size;
	g.data = data;
	g.entry_point = 0;
	return g;
}

void MachineCode::command(InstrSet cmd) {
	command(bin::join(static_cast<word>(cmd), static_cast<word>(0)));
}

void MachineCode::command(InstrSet cmd, InstrTarget t0) {
	command(bin::join(static_cast<word>(cmd), bin::join(static_cast<byte>(t0), static_cast<byte>(0))));
}

void MachineCode::command(InstrSet cmd, InstrTarget t0, InstrTarget t1) {
	command(bin::join(static_cast<word>(cmd), bin::join(static_cast<byte>(t0), static_cast<byte>(t1))));
}

void MachineCode::command(dword cmd) {
	code.emplace_back(Instr::State::cmd, cmd);
}

void MachineCode::go_to(byte label) {
	code.emplace_back(Instr::State::label_addr, label);
}

void MachineCode::label(byte id) {
	code.emplace_back(Instr::State::label, id);
}

void MachineCode::request_stack(udword s) {
	wanted_stack_size = s;
}

void MachineCode::request_bss(udword s) {
	wanted_bss_size = s;
}

void MachineCode::add_data(std::vector<byte> d) {
	for(auto const& b : d) {
		add_data_8(b);
	}
}

void MachineCode::add_data_8(byte b) {
	data.push_back(b);
}

void MachineCode::add_data_16(word b) {
	add_data_8(bin::lsb(b));
	add_data_8(bin::msb(b));
}

void MachineCode::add_data_32(dword b) {
	add_data_16(bin::lsb(b));
	add_data_16(bin::msb(b));
}

END_NS