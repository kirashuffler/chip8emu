#include "chip8.h"

//opcXXXX methods
void Chip8::opc0()
{
	switch (opcode->get000F())
	{
	case 0:
		Chip8::dispClear();
		drawFlag = true;
		break;
	case 0xE:
		//std::cout << "DBG: Popping value from stack: 0x" << std::hex << pcstack.top() << std::endl;
		pc->set(pcstack.top());
		pcstack.pop();
		break;
	}
}

void Chip8::opc1NNN()
{
	pc->set(opcode->get0FFF());
}

void Chip8::opc2NNN()
{
	pcstack.push(pc->get());
	//std::cout << "DBG: Pushing value on stack: 0x" << std::hex << pcstack.top() << std::endl;
	pc->set(opcode->get0FFF());
}

void Chip8::opc3XNN()
{
	if (V[opcode->getX()] == opcode->get00FF())
		pc->Incr();
}

void Chip8::opc4XNN()
{
	if (V[opcode->getX()] != opcode->get00FF())
		pc->Incr();
}

void Chip8::opc5XY0()
{
	if (V[opcode->getX()] == V[opcode->getY()])
		pc->Incr();
}

void Chip8::opc6XNN()
{
	V[opcode->getX()] = opcode->get00FF();
}

void Chip8::opc7XNN()
{
	V[opcode->getX()] += opcode->get00FF();
}

void Chip8::opc8()
{
	(this->*opcodes_8xy[opcode->get000F()])();
}

void Chip8::opc9XY0()
{
	if (V[opcode->getX()] != V[opcode->getY()])
		pc->Incr();
}

void Chip8::opcANNN()
{
	I = opcode->get0FFF();
}

void Chip8::opcBNNN()
{
	pc->set(V[0] + opcode->get0FFF());
}

void Chip8::opcCXNN()
{
	V[opcode->getX()] = (std::rand() % 0xFF) & (opcode->get00FF());
}

void Chip8::opcDXYN()
{
	gfxConfSprite(opcode->getX(), opcode->getY(), opcode->get000F());
}

void Chip8::opcE()
{
	switch (opcode->get00FF())
	{
	case 0x9E:
		if (key[V[opcode->getX()]] != 0) pc->Incr();
		break;
	case 0xA1:
		if (key[V[opcode->getY()]] == 0) pc->Incr();
		break;
	default:
		break;
	}
}

void Chip8::opcF()
{
	(this->*opcodes_fx[opcode->get00FF()])();
}

//opc8XXX methods (arithmetics)
void Chip8::opc8XY0()
{
	V[opcode->getX()] = V[opcode->getY()];
}

void Chip8::opc8XY1()
{
	V[opcode->getX()] |= V[opcode->getY()];
}

void Chip8::opc8XY2()
{
	V[opcode->getX()] &= V[opcode->getY()];
}

void Chip8::opc8XY3()
{
	V[opcode->getX()] ^= V[opcode->getY()];
}

void Chip8::opc8XY4()
{
	//Check if there is carry
	V[0xF] = (V[opcode->getY()] > (0xFF - V[opcode->getX()])) ? 1 : 0;
	V[opcode->getX()] += V[opcode->getY()];
}

void Chip8::opc8XY5()
{
	//Check if there is borrow
	V[0xF] = (V[opcode->getX()] < V[opcode->getY()]) ? 0 : 1;
	V[opcode->getX()] -= V[opcode->getY()];
}

void Chip8::opc8XY6()
{
	V[0xF] = V[opcode->getX()] & 0x1;
	V[opcode->getX()] >>= 1;
}

void Chip8::opc8XY7()
{
	V[0xF] = (V[opcode->getX()] > V[opcode->getY()]) ? 0 : 1;
	V[opcode->getX()] = V[opcode->getY()] - V[opcode->getX()];

}

void Chip8::opc8XYE()
{
	V[0xF] = V[opcode->getX()] >> 7;
	V[opcode->getX()] <<= 1;
}

void Chip8::opcFX07()
{
	V[opcode->getX()] = delay_timer; 
}

void Chip8::opcFX0A()
{
	uint8_t key_pressed = get_key();
	if (key_pressed >= 0)
		V[opcode->getX()] = key_pressed;
	else
		pc->Decr();
}

void Chip8::opcFX15()
{
	delay_timer = V[opcode->getX()];
}

void Chip8::opcFX18()
{
	V[opcode->getX()] = sound_timer;
}

void Chip8::opcFX1E()
{
	V[0xF] = (I + V[opcode->getX()] > 0xFFF) ? 1 : 0;
	I += V[opcode->getX()];
}

void Chip8::opcFX29()
{
	I = V[opcode->getX()] * 0x5;
}

void Chip8::opcFX33()
{
	memory[I] = V[opcode->getX()] / 100;
	memory[I + 1] = (V[opcode->getX()] % 100) / 10;
	memory[I + 2] = V[opcode->getX()] % 10;
}

void Chip8::opcFX55()
{
	for (uint8_t incr = 0; incr <= opcode->getX(); ++incr)
		memory[I + incr] = V[incr];
	//I += X + 1;
}

void Chip8::opcFX65()
{
	for (uint8_t incr = 0; incr <= opcode->getX(); ++incr)
		V[incr] = memory[I + incr];
	//I += X + 1;
}
		
