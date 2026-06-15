#include <iostream>
#include <SDL3/SDL.h>
#include "functions.h"
#include "defs.h"
#include <thread>
#include <vector>
#include <string>
#include <functional>
using namespace std;

const SDL_DisplayMode* info = NULL;
int width = 0;
int height = 0;
int bpp = 0;
int flags = 0;
SDL_Window* window;
SDL_Renderer* renderer;


void init()
{
	if (debug == true) { cout << "[DEBUG] function init from init.cpp" << endl; }
	// Initialize SDL's video subsystem and check for errors.
	if (SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		cout << "Video initialization failed: " << SDL_GetError() << endl;
		exit(1);
	}
	// Get information about the current video settings.
	info = SDL_GetDesktopDisplayMode(1);
	if (!info)
	{
		cout << "Video query failed: " << SDL_GetError() << endl;
		exit(1);
	}
	ScreenWidth = info->w;
	ScreenHeight = info->h;
	bpp = SDL_BITSPERPIXEL(info->format);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	flags = SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL;
}

void open_window() {
	if (debug == true) { cout << "[DEBUG] function open_window from init.cpp" << endl; }
	window = SDL_CreateWindow("OPENGL-TEST", ScreenWidth, ScreenHeight, SDL_WINDOW_OPENGL);
	renderer = SDL_CreateRenderer(window, nullptr);;
}