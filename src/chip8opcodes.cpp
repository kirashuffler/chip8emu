#include "chip8.h"
#include <iostream>
#include <iterator>

//opcXXXX methods
void Chip8::opc0()
{
	switch (opcode & 0xF)
	{
	case 0:
		Chip8::dispClear();
		drawFlag = true;
		break;
	case 0xE:
		//std::cout << "DBG: Popping value from stack: 0x" << std::hex << pcstack.top() << std::endl;
		pc = pcstack.top();
		pcstack.pop();
	}
}

void Chip8::opc1NNN()
{
	pc = opcode & 0x0FFF;
}

void Chip8::opc2NNN()
{
	pcstack.push(pc);
	//std::cout << "DBG: Pushing value on stack: 0x" << std::hex << pcstack.top() << std::endl;
	pc = opcode & 0x0FFF;
}

void Chip8::opc3XNN()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	if (V[X] == (opcode & 0x00FF))
		pc += 2;
}

void Chip8::opc4XNN()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	if (V[X] != (opcode & 0x00FF))
		pc += 2;
}

void Chip8::opc5XY0()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	uint8_t Y = (opcode & 0x00F0) >> 4;
	if (V[X] == V[Y])
		pc += 2;
}

void Chip8::opc6XNN()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	V[X] = opcode & 0x00FF;
}

void Chip8::opc7XNN()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	V[X] += opcode & 0x00FF;
}

void Chip8::opc8()
{
	(this->*opcodes_8xy[(opcode & 0x000F)])();
}

void Chip8::opc9XY0()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	uint8_t Y = (opcode & 0x00F0) >> 4;
	if (V[X] != V[Y])
		pc += 2;
}

void Chip8::opcANNN()
{
	I = opcode & 0x0FFF;
}

void Chip8::opcBNNN()
{
	pc = V[0] + 0x0FFF;
}

void Chip8::opcCXNN()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	V[X] = (std::rand() % 0xFF) & (opcode & 0x00FF);
}

void Chip8::opcDXYN()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	uint8_t Y = (opcode & 0x00F0) >> 4;
	uint8_t N = opcode & 0x000F;
	gfxConfSprite(X, Y, N);
}

void Chip8::opcE()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	switch (opcode & 0x00FF)
	{
	case 0x9E:
		pc += (key[V[X]] != 0) ? 2 : 0;
		break;
	case 0xA1:
		pc += (key[V[X]] == 0) ? 2 : 0;
		break;
	default:
		break;
	}
}

void Chip8::opcF()
{
	(this->*opcodes_fx[opcode & 0x00FF])();
}

//opc8XXX methods (arithmetics)
void Chip8::opc8XY0()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	uint8_t Y = (opcode & 0x00F0) >> 4;
	V[X] = V[Y];
}

void Chip8::opc8XY1()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	uint8_t Y = (opcode & 0x00F0) >> 4;
	V[X] |= V[Y];
}

void Chip8::opc8XY2()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	uint8_t Y = (opcode & 0x00F0) >> 4;
	V[X] &= V[Y];
}

void Chip8::opc8XY3()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	uint8_t Y = (opcode & 0x00F0) >> 4;
	V[X] ^= V[Y];
}

void Chip8::opc8XY4()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	uint8_t Y = (opcode & 0x00F0) >> 4;
	//Check if there is carry
	V[0xF] = (V[Y] > (0xFF - V[X])) ? 1 : 0;
	V[X] += V[Y];
}

void Chip8::opc8XY5()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	uint8_t Y = (opcode & 0x00F0) >> 4;
	//Check if there is borrow
	V[0xF] = (V[X] < V[Y]) ? 0 : 1;
	V[X] -= V[Y];
}

void Chip8::opc8XY6()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	uint8_t Y = (opcode & 0x00F0) >> 4;
	V[0xF] = V[X] & 0x1;
	V[X] >>= 1;
}

void Chip8::opc8XY7()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	uint8_t Y = (opcode & 0x00F0) >> 4;
	V[0xF] = (V[X] > V[Y]) ? 0 : 1;
	V[X] = V[Y] - V[X];

}

void Chip8::opc8XYE()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	uint8_t Y = (opcode & 0x00F0) >> 4;
	V[0xF] = V[X] >> 7;
	V[X] <<= 1;
}

void Chip8::opcFX07()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	V[X] = delay_timer; 
}

void Chip8::opcFX0A()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	uint8_t key_pressed = get_key();
	if (key_pressed >= 0)
		V[X] = key_pressed;
	else
		pc -= 2;
}

void Chip8::opcFX15()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	delay_timer = V[X];
}

void Chip8::opcFX18()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	V[X] = sound_timer;
}

void Chip8::opcFX1E()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	V[0xF] = (I + V[X] > 0xFFF) ? 1 : 0;
	I += V[X];
}

void Chip8::opcFX29()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	I = V[X] * 0x5;
}

void Chip8::opcFX33()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	memory[I] = V[X] / 100;
	memory[I + 1] = (V[X] % 100) / 10;
	memory[I + 2] = V[X] % 10;
}

void Chip8::opcFX55()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	for (uint8_t incr = 0; incr <= X; ++incr)
		memory[I + incr] = V[incr];
	//I += X + 1;
}

void Chip8::opcFX65()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	for (uint8_t incr = 0; incr <= X; ++incr)
		V[incr] = memory[I + incr];
	//I += X + 1;
}
		
