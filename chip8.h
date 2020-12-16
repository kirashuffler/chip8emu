#ifndef CHIP8
#define CHIP8

#include <functional>
#include <cstdint>
#include <stack>
namespace chip8 
{

#define MEMORY_SIZE 4096
#define GFX_WIDTH 64
#define GFX_HEIGHT 32
#define STACK_SIZE 16
#define KEYS_COUNT 16
#define REGS_COUNT 16

	class Chip8 {
	private:
		//pointer to opcode methods
		typedef void (Chip8::* OPCptr)();
		OPCptr opcodes_[0xE + 1];
		OPCptr opcodes_8xy[0xF + 1];
		OPCptr opcodes_fx[0x65 + 1];
		//Store current opcode
		uint16_t opcode;
		//4K Memory
		uint8_t memory[MEMORY_SIZE];
		//CPU regs
		uint8_t V[REGS_COUNT];
		//Index and PC regs
		uint16_t I, pc;
		//Graphics pixels states (0, 1) array.
		uint8_t gfx[GFX_WIDTH * GFX_HEIGHT];
		//timers
		uint8_t delay_timer, sound_timer;
		//stack to remember the current location before jump is performed
		std::stack <uint8_t> pcstack;
		//input keys 
		uint8_t key[KEYS_COUNT];
		void gfxConfSprite(uint8_t &x, uint8_t &y, uint8_t &n);
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
		void opcFX();
		void initOPCFunctors();
		void dispClear();
	public:
		Chip8();
		void emulateCycle();
	};






} //namespace chip8
#endif CHIP8
