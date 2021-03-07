#pragma once
#include "pch.h"
class ProgramCounter {
private:
	uint16_t value;
	static ProgramCounter* pc_;
	ProgramCounter(uint16_t val) : value(val) {};
public:
	ProgramCounter(ProgramCounter& other) = delete;
	void operator=(const ProgramCounter&) = delete;
	static ProgramCounter* GetInstance(const uint16_t value);
	uint16_t get() const;
	void set(uint16_t val);
	void Incr();//to the next instruction
	void Decr();//to the prev instruction
};

ProgramCounter* ProgramCounter::pc_ = nullptr;

