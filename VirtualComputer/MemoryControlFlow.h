#pragma once

#include "config.h"

BEGIN_NS

class MemoryControlFlow {
public:
	virtual ~MemoryControlFlow() {}

	virtual void read() = 0;
	virtual void write() = 0;

	void read_write() {
		read();
		write();
	}
};

END_NS
