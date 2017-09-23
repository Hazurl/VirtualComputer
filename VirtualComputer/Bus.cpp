#include "stdafx.h"
#include "Bus.h"

BEGIN_NS

/*
 * Bus 8
 */

void Bus8::bind(byte b) {
	storage = b;
}

byte Bus8::extract() const {
	return storage;
}


Bus8::bit_t Bus8::bit(ubyte b) {
	return bit_t(storage, b);
}

/*
 * Bus 16
 */

void Bus16::bind(word b) {
	l.bind(b & 0xFF);
	h.bind(b >> 8);
}

void Bus16::bind_h(byte b) {
	h.bind(b);
}

void Bus16::bind_l(byte b) {
	l.bind(b);
}

void Bus16::bind(byte h, byte l) {
	this->h.bind(h);
	this->l.bind(l);
}

byte Bus16::extract_h() const {
	return h.extract();
}

byte Bus16::extract_l() const {
	return l.extract();
}

word Bus16::extract() const {
	return (static_cast<ubyte>(h.extract()) << 8) | static_cast<ubyte>(l.extract());
}

Bus8::bit_t Bus16::bit(ubyte p) {
	if(p > 7)
		return h.bit(p - 8);
	return l.bit(p);
}

Bus8& Bus16::bus_h() {
	return h;
}

Bus8& Bus16::bus_l() {
	return l;
}

/*
* Bus 32
*/


void Bus32::bind(byte hh, byte hl, byte lh, byte ll) {
	h.bind(hh, hl);
	l.bind(lh, ll);
}

void Bus32::bind_hh(byte hh) {
	h.bind_h(hh);
}

void Bus32::bind_hl(byte hl) {
	h.bind_l(hl);
}

void Bus32::bind_lh(byte lh) {
	l.bind_h(lh);
}

void Bus32::bind_ll(byte ll) {
	l.bind_l(ll);
}

void Bus32::bind_h(word h) {
	this->h.bind(h);
}

void Bus32::bind_l(word l) {
	this->l.bind(l);
}

void Bus32::bind(word h, word l) {
	this->h.bind(h);
	this->l.bind(l);
}

void Bus32::bind(dword b) {
	h.bind(b >> 16);
	l.bind(b & 0xFFFF);
}

byte Bus32::extract_hh() const {
	return h.extract_h();
}

byte Bus32::extract_hl() const {
	return h.extract_l();
}

byte Bus32::extract_lh() const {
	return l.extract_h();
}

byte Bus32::extract_ll() const {
	return l.extract_l();
}

word Bus32::extract_h() const {
	return h.extract();
}

word Bus32::extract_l() const {
	return l.extract();
}

dword Bus32::extract() const {
	return (static_cast<uword>(h.extract()) << 16) | static_cast<uword>(l.extract());
}

Bus8::bit_t Bus32::bit(ubyte p) {
	if(p > 15)
		return h.bit(p - 16);
	return l.bit(p);
}

Bus8& Bus32::bus_hh() {
	return h.bus_h();
}

Bus8& Bus32::bus_hl() {
	return h.bus_l();
}

Bus8& Bus32::bus_lh() {
	return l.bus_h();
}

Bus8& Bus32::bus_ll() {
	return l.bus_l();
}

Bus16& Bus32::bus_h() {
	return h;
}

Bus16& Bus32::bus_l() {
	return l;
}

END_NS

