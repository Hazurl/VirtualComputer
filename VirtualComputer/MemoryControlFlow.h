#pragma once

#include "config.h"

BEGIN_NS

class MemoryControlFlow {
public:
	virtual ~MemoryControlFlow() {}

	virtual void read() = 0;
	virtual void write() = 0;
};

END_NS
