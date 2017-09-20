#pragma once

#include <limits>
#include <cstdint>

#define NAMESPACE vphaz
#define BEGIN_NS namespace vphaz {
#define END_NS }

BEGIN_NS

using byte = int8_t;
using ubyte = uint8_t;

constexpr byte byte_min = std::numeric_limits<int8_t>::min();
constexpr byte byte_max = std::numeric_limits<int8_t>::max();

constexpr ubyte ubyte_min = std::numeric_limits<uint8_t>::min();
constexpr ubyte ubyte_max = std::numeric_limits<uint8_t>::max();

END_NS