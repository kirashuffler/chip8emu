#include "chip8.h"
#include "sdl_viewer.h"

void Run(const char* path) 
{
	uint16_t width = GFX_WIDTH;
	uint16_t height = GFX_HEIGHT;

	SDLViewer viewer("CHIP-8 Emulator", width, height, 8);
	uint8_t* rgb24 = static_cast<uint8_t*>(std::calloc(width * height * 3, sizeof(uint8_t)));
	viewer.SetFrameRGB24(rgb24, height);
	Chip8 cpu;
	cpu.loadApplication(path);
	
}


