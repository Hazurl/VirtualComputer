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

using word = int16_t;
using uword = uint16_t;

constexpr word dword_min = std::numeric_limits<int16_t>::min();
constexpr word dword_max = std::numeric_limits<int16_t>::max();

constexpr uword uword_min = std::numeric_limits<uint16_t>::min();
constexpr uword uword_max = std::numeric_limits<uint16_t>::max();

using dword = int32_t;
using udword = uint32_t;

constexpr dword dword_min = std::numeric_limits<int32_t>::min();
constexpr dword dword_max = std::numeric_limits<int32_t>::max();

constexpr udword udword_min = std::numeric_limits<uint32_t>::min();
constexpr udword udword_max = std::numeric_limits<uint32_t>::max();

END_NS