#include "chip8.h"

Chip8* Chip8::chip8_ = nullptr;

Chip8* Chip8::GetInstance(const std::string& name) {
	if (chip8_ == nullptr)
		chip8_ = new Chip8(name);
	return chip8_;
}

void Chip8::debugRender()
{
	// Draw
	for(int y = 0; y < 32; ++y)
	{
		for(int x = 0; x < 64; ++x)
		{
			if(gfx[(y*64) + x] == 0) 
				printf("O");
			else 
				printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}


void Chip8::initOPCFunctors()
{
	opcodes_[0x0] = &Chip8::opc0;
	opcodes_[0x1] = &Chip8::opc1NNN;
	opcodes_[0x2] = &Chip8::opc2NNN;
	opcodes_[0x3] = &Chip8::opc3XNN;
	opcodes_[0x4] = &Chip8::opc4XNN;
	opcodes_[0x5] = &Chip8::opc5XY0;
	opcodes_[0x6] = &Chip8::opc6XNN;
	opcodes_[0x7] = &Chip8::opc7XNN;
	opcodes_[0x8] = &Chip8::opc8;
	opcodes_[0x9] = &Chip8::opc9XY0;
	opcodes_[0xA] = &Chip8::opcANNN;
	opcodes_[0xB] = &Chip8::opcBNNN;
	opcodes_[0xC] = &Chip8::opcCXNN;
	opcodes_[0xD] = &Chip8::opcDXYN;
	opcodes_[0xE] = &Chip8::opcE;
	opcodes_[0xF] = &Chip8::opcF;
	// arithmetic opcodes
	opcodes_8xy[0x0] = &Chip8::opc8XY0;
	opcodes_8xy[0x1] = &Chip8::opc8XY1;
	opcodes_8xy[0x2] = &Chip8::opc8XY2;
	opcodes_8xy[0x3] = &Chip8::opc8XY3;
	opcodes_8xy[0x4] = &Chip8::opc8XY4;
	opcodes_8xy[0x5] = &Chip8::opc8XY5;
	opcodes_8xy[0x6] = &Chip8::opc8XY6;
	opcodes_8xy[0x7] = &Chip8::opc8XY7;
	opcodes_8xy[0xE] = &Chip8::opc8XYE;
	// FX opcodes
	opcodes_fx[0x07] = &Chip8::opcFX07;
	opcodes_fx[0x0A] = &Chip8::opcFX0A;
	opcodes_fx[0x15] = &Chip8::opcFX15;
	opcodes_fx[0x18] = &Chip8::opcFX18;
	opcodes_fx[0x1E] = &Chip8::opcFX1E;
	opcodes_fx[0x29] = &Chip8::opcFX29;
	opcodes_fx[0x33] = &Chip8::opcFX33;
	opcodes_fx[0x55] = &Chip8::opcFX55;
	opcodes_fx[0x65] = &Chip8::opcFX65;
}

void Chip8::init()
{
	waiting_for_key = false;
	pc->set(0x200);
	opcode->set(0);
	I = 0;
	std::memset(memory, 0, MEMORY_SIZE);
	std::memset(gfx, 0, GFX_HEIGHT * GFX_WIDTH);
	std::memset(V, 0, REGS_COUNT);
	std::memcpy(memory, chip8_fontset, FONTS_COUNT);
	drawFlag = true;
	initOPCFunctors();
}

void Chip8::fetchOPC()
{
	if (pc->get() < 0x200)
		std::cerr << "WRONG MEM ACCESS";
	opcode->set((memory[pc->get()] << 8) | memory[pc->get() + 1]);
	pc->Incr();
}

void Chip8::dispClear()
{
	for (uint16_t i = 0; i < GFX_HEIGHT * GFX_WIDTH; ++i)
		gfx[i] = 0;
}

void Chip8::gfxConfSprite(const uint8_t &x,const uint8_t &y,const uint8_t &n)
{
	uint8_t xPos = V[x] % GFX_WIDTH;
	uint8_t yPos = V[y] % GFX_HEIGHT;
	uint8_t height = n;
	V[0xF] = 0;
	for (size_t row = 0; row < height; ++row)
	{
		uint8_t spriteByte = memory[I + row];
		for (size_t col = 0; col < 8; ++col)
		{
			uint8_t spritePixel = spriteByte & (0x80 >> col);
			uint32_t* screenPixel = &gfx[(yPos + row) * GFX_WIDTH + (xPos + col)];

			if (spritePixel)
			{
				if (*screenPixel == 0xFFFFFFFF)
					V[0xF] = 1;
			*screenPixel ^= 0xFFFFFFFF;
			}
		}
		std::bitset<8> bitrep(spriteByte);
	}
	drawFlag = true;
}

void Chip8::execOPC()
{
	fetchOPC();
	(this->*opcodes_[opcode->getF000()])();
}

void Chip8::emulateCycle()
{
	execOPC();
	std::this_thread::sleep_for(std::chrono::milliseconds(16));
	delay_timer -= (delay_timer > 0) ? 1 : 0;
	if (sound_timer > 0)
	{
		if (sound_timer == 1)
			std::cout << "BEEP!" << std::endl;
		--sound_timer;
	}
}

int Chip8::get_key()
{
	for (uint8_t i = 0; i < KEYS_COUNT; ++i)
		if (key[i] > 0)
		{
			waiting_for_key = false;
			return i;
		}
	waiting_for_key = true;
	return -1;
}

void Chip8::keyboardUp(unsigned char inputKey, int x, int y)
{
	key[key_map[inputKey]] = 0;
}

void Chip8::keyboardDown(unsigned char inputKey,int x, int y)
{
	key[key_map[inputKey]] = 1;
}

bool Chip8::loadApplication(const char * filename)
{
	init();
	std::cout << "Loading: " << filename << std::endl;
	FILE * pFile = fopen(filename, "rb");
	if (pFile == NULL)
	{
		std::cerr << "File error";
		return false;
	}
	fseek(pFile, 0, SEEK_END);
	uint16_t lSize = ftell(pFile);
	rewind(pFile);
	std::cout << "Filesize: " << lSize << std::endl;
	char *buffer = (char*)malloc(sizeof(char) * lSize);
	if (buffer == NULL)
	{
		std::cerr << "Memory error" << std::endl;
		return false;
	}

	size_t result = fread (buffer, 1, lSize, pFile);
	if (result != lSize)
	{
		std::cerr << "Reading error" << std::endl;
		return false;
	}
	if ((4096 - 512) > lSize)
	{
		for (uint16_t i = 0; i < lSize; ++i)
			memory[i + 512] = buffer[i];
	}
	else 
	{
		std::cerr << "ROM is too big for memory" << std::endl;
	}
	fclose(pFile);
	free(buffer);
	return true;
}
