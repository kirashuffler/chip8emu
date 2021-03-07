#include "OPCode.h"

OPCode* OPCode::GetInstance(const uint16_t value) {
	if (opcode_ == nullptr) {
		opcode_ = new OPCode(value);
	}
	return opcode_;
}

uint8_t OPCode::getX() {
	return (value & 0x0F00) >> 8;
}

uint16_t OPCode::getF000() {
	return (value & 0xF000) >> 12;
}

uint8_t OPCode::get000F() {
	return value & 0x000F;
}

uint16_t OPCode::get00FF() {
	return (value & 0x00FF);
}

uint16_t OPCode::get0FFF() {
	return (value & 0xFFF);
}

uint8_t OPCode::getX() {
	return (value & 0x0F00) >> 8;
}

uint8_t OPCode::getY() {
	return (value & 0x00F0) >> 4;
}

uint16_t OPCode::get() const {
	return value;
}

void OPCode::set(uint16_t val) {
	value = val;
}
