#include "stdafx.h"
#include "MachineCode.h"

BEGIN_NS

MachineCode::Instr::Instr(Instr:: State s, dword b) : state(s), cmd(b) {

}

std::vector<dword> MachineCode::generate(dword* linker) {
	std::vector<dword> gen;
	std::unordered_map<byte, byte> labels; // id -> mem
	std::unordered_map<byte, byte> go_tos; // id -> mem
	for(auto& i : code) {
		switch(i.state) {
		case Instr::State::cmd:
			gen.emplace_back(i.cmd);
			break;
		case Instr::State::link:
			gen.emplace_back(linker[i.cmd]);
			break;
		case Instr::State::label:
			labels.emplace(i.cmd, (gen.size() - 1) * sizeof(dword));
			break;
		case Instr::State::label_addr:
			auto it = labels.find(i.cmd);
			if(it != labels.end())
				gen.emplace_back(it->second + sizeof(dword));
			else {
				go_tos.emplace(i.cmd, gen.size());
				gen.emplace_back(0);
			}
			break;
		}
	}

	for(auto& p : go_tos) {
		auto it = labels.find(p.first);
		if(it == labels.end())
			return {};
		gen[p.second] = it->second;
	}

	return gen;
}

void MachineCode::command(InstrSet cmd) {
	command(static_cast<dword>(cmd));
}

void MachineCode::command(dword cmd) {
	code.emplace_back(Instr::State::cmd, cmd);
}

void MachineCode::go_to(byte label) {
	code.emplace_back(Instr::State::label_addr, label);
}

void MachineCode::link(byte id) {
	code.emplace_back(Instr::State::link, id);
}

void MachineCode::label(byte id) {
	code.emplace_back(Instr::State::label, id);
}

END_NS