#pragma once
#include "pch.h"

class OPCode {
private:
	uint16_t value;
	static OPCode* opcode_;
	OPCode(uint16_t val) : value(val) {};
public:
	OPCode(OPCode& other) = delete;
	void operator=(const OPCode&) = delete;
	static OPCode* GetInstance(const uint16_t value);
	void set(uint16_t val);
	uint16_t get() const;
	uint16_t getF000();
	uint8_t get000F();
	uint16_t get0FFF();
	uint16_t get00FF();
	uint8_t getX();
	uint8_t getY();
};

OPCode* OPCode::opcode_ = nullptr;

