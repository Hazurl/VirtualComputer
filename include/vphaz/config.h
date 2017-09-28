#pragma once

#include <limits>
#include <cstdint>

#define NAMESPACE vphaz
#define BEGIN_NS namespace vphaz {
#define END_NS }

BEGIN_NS

template<typename T>
struct unsigned_t_helper;
template<typename T>
struct signed_t_helper;

#define TYPE_MIN_MAX(t, s)\
using t = int ## s ## _t;\
using u ## t = uint ## s ## _t;\
constexpr t t ## _min = std::numeric_limits<int ## s ## _t>::min();\
constexpr t t ## _max = std::numeric_limits<int ## s ## _t>::max();\
constexpr u ## t u ## t ## _min = std::numeric_limits<uint ## s ## _t>::min();\
constexpr u ## t u ## t ## _max = std::numeric_limits<uint ## s ## _t>::max();\
template<>\
struct unsigned_t_helper<t> {\
	using type = u ## t; \
};\
template<>\
struct unsigned_t_helper<u ## t> {\
	using type = u ## t; \
};\
template<>\
struct signed_t_helper<t> {\
	using type = t; \
};\
template<>\
struct signed_t_helper<u ## t> {\
	using type = t; \
};


template<typename T>
using unsigned_t = typename unsigned_t_helper<T>::type;
template<typename T>
using signed_t = typename signed_t_helper<T>::type;

TYPE_MIN_MAX(byte, 8)
TYPE_MIN_MAX(word, 16)
TYPE_MIN_MAX(dword, 32)
TYPE_MIN_MAX(xdword, 64)

template<typename T>
inline unsigned_t<T> us_cast(T const& t) {
	return static_cast<unsigned_t<T>>(t);
}

template<typename T>
inline signed_t<T> s_cast(T const& t) {
	return static_cast<signed_t<T>>(t);
}

END_NS