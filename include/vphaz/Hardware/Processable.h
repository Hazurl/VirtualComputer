#pragma once

#include <vphaz/config.h>

BEGIN_NS

class Processable {
public:
	virtual ~Processable() {}

	virtual void process() = 0;
};

END_NS