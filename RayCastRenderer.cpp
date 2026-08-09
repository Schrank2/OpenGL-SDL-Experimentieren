#include <SDL3/SDL.h>
#include "defs.h"
#include "ray.h"
#include <vector>
#include <thread>
#include <mutex>
#include <string>

void RAY::init(int* ScreenWidth, int* ScreenHeight, float* WindowSize, string WindowTitle) {
	// Store WindowSize
	WindowWidthF = static_cast<float>(*ScreenWidth) * *WindowSize;
	WindowHeightF = static_cast<float>(*ScreenHeight) * *WindowSize;
	// Create WindowTitle
	flags = SDL_WINDOW_ALWAYS_ON_TOP;
	WindowTitle = WindowTitle + " " + to_string(WindowWidthF) + "x" + to_string(WindowHeightF);
	const char* WindowTitleChar = WindowTitle.c_str();
	// Create Window
	window = SDL_CreateWindow(WindowTitleChar, WindowWidthF, WindowHeightF, flags);
	if (!window)
	{
		cout << "Window creation failed: " << SDL_GetError() << endl;
		exit(1);
	}
	// Create Renderer
	renderer = SDL_CreateRenderer(window, NULL);
	if (!renderer)
	{
		cout << "Renderer creation failed: " << SDL_GetError() << endl;
		exit(1);
	}
}

void RAY::render() {

}

RAY ray;