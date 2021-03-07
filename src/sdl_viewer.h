#pragma once
#include "pch.h"
class SDLViewer {
public:
	SDLViewer(const std::string& title, uint16_t width, uint16_t height, uint16_t window_scale = 1);
	~SDLViewer();

	std::vector<SDL_Event> Update();
	void SetFrameRGB24(uint8_t* rgb24, uint16_t height);
private:
	std::string title_;
	SDL_Window* window_ = nullptr;
	SDL_Renderer* renderer_ = nullptr;
	SDL_Texture* window_tex_ = nullptr;
};
