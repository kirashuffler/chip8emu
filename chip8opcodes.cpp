#include <chip8.h>

namespace chip8 
{
	//opcXXXX methods
	void Chip8::opc0()
	{
		switch (opcode & 0xF)
		{
		case 0:
			Chip8::dispClear();
			break;
		case 0xE:
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
		V[X] = std::rand() & (opcode & 0x00FF);
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
		V[0xF] |= V[X] & 0x1;
		V[X] >>= 1;
	}

	void Chip8::opc8XY7()
	{
		uint8_t X = (opcode & 0x0F00) >> 8;
		uint8_t Y = (opcode & 0x00F0) >> 4;
		V[X] = V[Y] - V[X];

	}

	void Chip8::opc8XYE()
	{
		uint8_t X = (opcode & 0x0F00) >> 8;
		uint8_t Y = (opcode & 0x00F0) >> 4;
		V[0xF] = (V[X] >> 7) & 0x1;
		V[X] = V[Y];
	}
}//namespace chip8