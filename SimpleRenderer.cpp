#include <SDL3/SDL.h>
#include "functions.h"
#include "defs.h"
#include "class.h"
#include <vector>

void SimpleRenderer::open_window() {
	if (debug == true) { cout << "[DEBUG] function simple.open_window() from SimpleRenderer.cpp" << endl; }
	string WindowTitle;
	const SDL_DisplayMode* info = SDL_GetDesktopDisplayMode(1);
	if (!info)
	{
		cout << "Video query failed: " << SDL_GetError() << endl;
		exit(1);
	}
	// Get Screen Information
	ScreenWidth = info->w / 2;
	ScreenHeight = info->h / 2;
	flags = SDL_WINDOW_RESIZABLE;
	// creating the title for the application window
	WindowTitle = "Simple Renderer " + to_string(ScreenWidth) + "x" + to_string(ScreenHeight);
	char* WindowTitleChar = &WindowTitle[0];
	// creating the window
	simple.window = SDL_CreateWindow(WindowTitleChar, ScreenWidth, ScreenHeight, flags);
	if (!simple.window)
	{
		cout << "Window creation failed: " << SDL_GetError() << endl;
		exit(1);
	}
	// creating the renderer
	simple.renderer = SDL_CreateRenderer(window, nullptr);
	if (!simple.renderer)
	{
		cout << "Renderer creation failed: " << SDL_GetError() << endl;
		exit(1);
	}
}

void SimpleRenderer::render() {
	if (debug == true) { cout << "[DEBUG] function simple.render() from SimpleRenderer.cpp" << endl; }
	SDL_SetRenderDrawColor(simple.renderer, 0, 0, 0, 255);
	SDL_RenderClear(simple.renderer);
	simple.draw();
	SDL_RenderPresent(simple.renderer);
}

void SimpleRenderer::draw() {
	if (debug == true) { cout << "[DEBUG] function simple.draw() from SimpleRenderer.cpp" << endl; }
	SDL_SetRenderDrawColor(simple.renderer, 255, 0, 0, 255);
	SDL_RenderLine(simple.renderer, 30, 30, 50, 50);
}

SimpleRenderer simple;