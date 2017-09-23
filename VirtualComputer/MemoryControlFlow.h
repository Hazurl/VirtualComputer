#pragma once

#include "config.h"

BEGIN_NS

class MemoryControlFlow8 {
public:
	virtual ~MemoryControlFlow8() {}

	virtual void read8() = 0;

	virtual void write8() = 0;

	void read_write8() {
		read8();
		write8();
	}
};

class MemoryControlFlow16 : public MemoryControlFlow8 {
public:
	virtual ~MemoryControlFlow16() {}

	virtual void read16() = 0;

	virtual void write16() = 0;

	virtual void write8h() = 0;
	virtual void read8h() = 0;

	void read_write8h() {
		read8h();
		write8h();
	}

	void read_write16() {
		read16();
		write16();
	}
};

class MemoryControlFlow32 : public MemoryControlFlow16 {
public:
	virtual ~MemoryControlFlow32() {}

	virtual void read32() = 0;
	virtual void write32() = 0;

	virtual void read16h() = 0;
	virtual void write16h() = 0;

	void read_write16h() {
		read16h();
		write16h();
	}

	void read_write32() {
		read32();
		write32();
	}
};

END_NS
