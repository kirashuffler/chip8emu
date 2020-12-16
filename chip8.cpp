#include "chip8.h"
#include <stdlib.h>
#include <iostream>

namespace chip8
{
	void Chip8::initOPCFunctors()
	{
		opcodes_[0x0] = opc0;
		opcodes_[0x1] = opc1NNN;
		opcodes_[0x2] = opc2NNN;
		opcodes_[0x3] = opc3XNN;
		opcodes_[0x4] = opc4XNN;
		opcodes_[0x5] = opc5XY0;
		opcodes_[0x6] = opc6XNN;
		opcodes_[0x7] = opc7XNN;
		opcodes_[0x8] = opc8;
		opcodes_[0x9] = opc9XY0;
		opcodes_[0xA] = opcANNN;
		opcodes_[0xB] = opcBNNN;
		opcodes_[0xC] = opcCXNN;
		opcodes_[0xD] = opcDXYN;
		opcodes_[0xE] = opcE;
		opcodes_[0xF] = opcF;
		// arithmetic opcodes
		opcodes_8xy[0x0] = opc8XY0;
		opcodes_8xy[0x1] = opc8XY1;
		opcodes_8xy[0x2] = opc8XY2;
		opcodes_8xy[0x3] = opc8XY3;
		opcodes_8xy[0x4] = opc8XY4;
		opcodes_8xy[0x5] = opc8XY5;
		opcodes_8xy[0x6] = opc8XY6;
		opcodes_8xy[0x7] = opc8XY7;
		opcodes_8xy[0xE] = opc8XYE;
	}

	Chip8::Chip8()
	{
		pc = 0x200;
		opcode = 0;
		I = 0;
		for (uint16_t i = 0; i < MEMORY_SIZE; ++i)
			memory[i] = 0;
		for (uint16_t i = 0; i < GFX_HEIGHT * GFX_WIDTH; ++i)
			gfx[i] = 0;
		for (uint16_t i = 0; i < REGS_COUNT; ++i)
			V[i] = 0;
	}

	void Chip8::fetchOPC()
	{
		opcode = (memory[pc] << 8) | memory[pc + 1];
		pc += 2;
	}
	void Chip8::dispClear()
	{
		for (uint16_t i = 0; i < GFX_HEIGHT * GFX_WIDTH; ++i)
			gfx[i] = 0;
	}
	
	void Chip8::gfxConfSprite(uint8_t &x, uint8_t &y, uint8_t &n)
	{
		uint8_t width = 8;
		uint8_t height = n + 1;
		uint8_t start_pos = GFX_WIDTH * y + x;
		uint16_t mem_addr = I;
		uint8_t VFStatus = 0;
		for (uint8_t i = 0; i < height; ++i)
		{
			uint8_t line = memory[mem_addr + i];
			for (uint8_t bit = 0; bit < width; ++bit)
			{
				try 
				{
					uint8_t prevPixelState = gfx[start_pos + GFX_WIDTH * i + bit]; 
					gfx[start_pos + GFX_WIDTH * i + bit] ^= ((line & (1 << bit)) << bit);
					VFStatus += ((prevPixelState != 0) && (gfx[start_pos + GFX_WIDTH * i + bit] == 0)) ? 1 : 0;
				}
				catch (...)
				{
					std::cout << "GFX Error: Boundaries of GFX array are violated.";
				}				
			}
		}
		V[0xF] = (VFStatus > 0) ? 1 : 0;
	}

	void Chip8::execOPC()
	{
		fetchOPC();
		(this->*opcodes_[(opcode >> 12)])();

	}
	void Chip8::emulateCycle()
	{
	}


}//namespace chip8
