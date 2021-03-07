#include "ProgramCounter.h"

ProgramCounter* ProgramCounter::GetInstance(const uint16_t value) {
	if (pc_ == nullptr) {
		pc_ = new ProgramCounter(value);
	}
	return pc_;
}

void ProgramCounter::set(uint16_t val) {
	value = val;
}

uint16_t ProgramCounter::get() const {
	return value;
}

void ProgramCounter::Incr() {
	value += 2;
}

void ProgramCounter::Decr() {
	value -= 2;
}

