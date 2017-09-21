#pragma once

#include "config.h"
#include "Processable.h"
#include "Bus.h"

BEGIN_NS

class OutConsole : public Processable {
public:
	OutConsole(Bus* bus_data);

	void process() override;

private:

	Bus* bus_data;
};

END_NS