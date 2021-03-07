#pragma once
#include "pch.h"
#include "OPCode.h"
#include "ProgramCounter.h"

constexpr uint16_t MEMORY_SIZE = 4096;
constexpr uint8_t GFX_WIDTH = 64;
constexpr uint8_t GFX_HEIGHT = 32;
constexpr uint8_t STACK_SIZE = 16;
constexpr uint8_t  KEYS_COUNT = 16;
constexpr uint8_t  REGS_COUNT = 16;
constexpr uint8_t  FONTS_COUNT = 80;

class Chip8 {
private:
	std::string filename;
	Chip8(const std::string name) : filename(name) {};
	static Chip8* chip8_;
	//pointer to opcode methods
	typedef void (Chip8::* OPCptr)();
	OPCptr opcodes_[0xF + 1];
	OPCptr opcodes_8xy[0xF + 1];
	OPCptr opcodes_fx[0x65 + 1];
	//Store current opcode
	OPCode* opcode = OPCode::GetInstance(0);
	//4K Memory
	uint8_t memory[MEMORY_SIZE];
	//CPU regs
	uint8_t V[REGS_COUNT];
	//Index and PC regs
	uint16_t I;
	ProgramCounter* pc = ProgramCounter::GetInstance(0);
	//timers
	uint8_t delay_timer, sound_timer;
	//stack to remember the current location before jump is performed
	std::stack<uint16_t> pcstack;
	//waiting for key input
	bool waiting_for_key = false;
	bool drawFlag = false;
	//Graphics pixels states (0, 1) array.
	uint32_t gfx[GFX_WIDTH * GFX_HEIGHT];
	//input keys 
	uint8_t key[KEYS_COUNT];

	std::map<char,uint8_t> key_map =
	{
		{'1', 0x1},
		{'2', 0x2},
		{'3', 0x3},
		{'4', 0xC},

		{'q', 0x4},
		{'w', 0x5},
		{'e', 0x6},
		{'r', 0xD},
		
		{'a', 0x7},
		{'s', 0x8},
		{'d', 0x9},
		{'f', 0xE},

		{'z', 0xA},
		{'x', 0x0},
		{'c', 0xB},
		{'v', 0xF},
	};

	unsigned char chip8_fontset[FONTS_COUNT] =
	{ 
		0xF0, 0x90, 0x90, 0x90, 0xF0, //0
		0x20, 0x60, 0x20, 0x20, 0x70, //1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
		0x90, 0x90, 0xF0, 0x10, 0x10, //4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
		0xF0, 0x10, 0x20, 0x40, 0x40, //7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
		0xF0, 0x90, 0xF0, 0x90, 0x90, //A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
		0xF0, 0x80, 0x80, 0x80, 0xF0, //C
		0xE0, 0x90, 0x90, 0x90, 0xE0, //D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
		0xF0, 0x80, 0xF0, 0x80, 0x80  //F
	};

	void gfxConfSprite(const uint8_t &x,const uint8_t &y,const uint8_t &n);
	void fetchOPC();
	void execOPC();

	void opc0();
	void opc1NNN();
	void opc2NNN();
	void opc3XNN();
	void opc4XNN();
	void opc5XY0();
	void opc6XNN();
	void opc7XNN();
	void opc8();
	void opc9XY0();
	void opcANNN();
	void opcBNNN();
	void opcCXNN();
	void opcDXYN();
	void opcE();
	void opcF();
	//opc8 opcode methods
	void opc8XY0();
	void opc8XY1();
	void opc8XY2();
	void opc8XY3();
	void opc8XY4();
	void opc8XY5();
	void opc8XY6();
	void opc8XY7();
	void opc8XYE();
	//opcF opcode methods
	void opcFX07();
	void opcFX0A();
	void opcFX15();
	void opcFX18();
	void opcFX1E();
	void opcFX29();
	void opcFX33();
	void opcFX55();
	void opcFX65();
	
	void initOPCFunctors();
	void dispClear();
	int get_key();
	void debugRender();
	void emulateCycle();
	void keyboardUp(unsigned char inputKey, int x, int y);
	void keyboardDown(unsigned char inputKey, int x, int y);
	bool loadApplication(const char * filename);

public:
	Chip8(Chip8& other) = delete;
	void operator=(const Chip8&) = delete;
	static Chip8* GetInstance(const std::string& name);
	void init();
};

Chip8* Chip8::chip8_ = nullptr;

