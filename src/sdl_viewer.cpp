#include "sdl_viewer.h"

SDLViewer::SDLViewer(const std::string& title, uint16_t width, uint16_t height, uint16_t window_scale) :
	title_(title)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error(SDL_GetError());
	}
	window_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width * window_scale, height * window_scale, SDL_WINDOW_SHOWN);
	renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_WINDOW_SHOWN);
	SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);

	window_tex_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, width, height);
}

SDLViewer::~SDLViewer()
{
	SDL_DestroyTexture(window_tex_);
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
	SDL_Quit();
}

std::vector<SDL_Event> SDLViewer::Update()
{
	std::vector<SDL_Event> events;
	SDL_Event e;
	while (SDL_PollEvent(&e))
		events.push_back(e);
	SDL_RenderCopy(renderer_, window_tex_, NULL, NULL);
	SDL_RenderPresent(renderer_);

	return events;
}

void SDLViewer::SetFrameRGB24(uint8_t* rgb24, uint16_t height)
{
	void* pixeldata;
	int pitch;
	SDL_LockTexture(window_tex_, nullptr, &pixeldata, &pitch);
	std::memcpy(pixeldata, rgb24, pitch * height);
	SDL_UnlockTexture(window_tex_);
}
