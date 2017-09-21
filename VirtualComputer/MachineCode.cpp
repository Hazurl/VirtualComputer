#include "stdafx.h"
#include "MachineCode.h"

BEGIN_NS

MachineCode::Instr::Instr(Instr:: State s, byte b) : state(s), cmd(b) {

}

std::vector<byte> MachineCode::generate(byte* linker) {
	std::vector<byte> gen;
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
			labels.emplace(i.cmd, gen.size() - 1);
			break;
		case Instr::State::label_addr:
			auto it = labels.find(i.cmd);
			if(it != labels.end())
				gen.emplace_back(it->second + 1);
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
		gen[p.second] = it->second + 1;
	}

	return gen;
}

void MachineCode::command(CU::OP cmd) {
	command(static_cast<byte>(cmd));
}

void MachineCode::command(byte cmd) {
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