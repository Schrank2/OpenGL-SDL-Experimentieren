#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "functions.h"
#include "defs.h"
#include "class.h"
#include <string>
using namespace std;

const SDL_DisplayMode* info = NULL;
int ScreenWidth = 0;
int ScreenHeight = 0;
float ScreenWidthF = 0.0f;
float ScreenHeightF = 0.0f;
int bpp = 0;
int flags = 0;
SDL_Window* window;
SDL_Renderer* renderer;
SDL_GLContext OpenglContext;
string WindowTitle;


void InitSdl()
{
	if (debug == true) { cout << "[DEBUG] function InitSdl() from init.cpp" << endl; }
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
}

void open_window() {
	if (debug == true) { cout << "[DEBUG] function open_window() from init.cpp" << endl; }
	// Set desired GL attributes before creating window/context.
	// Adjust version/profile as needed for your GPU/drivers.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	ScreenWidth = info->w / 2;
	ScreenHeight = info->h / 2;
	ScreenWidthF = static_cast<float>(ScreenWidth);
	ScreenHeightF = static_cast<float>(ScreenHeight);
	bpp = SDL_BITSPERPIXEL(info->format);
	flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
	// creating the title for the application window
	WindowTitle = "OPENGL-TEST " + to_string(ScreenWidth) + "x" + to_string(ScreenHeight);
	char* WindowTitleChar = &WindowTitle[0];
	// creating the window
	window = SDL_CreateWindow(WindowTitleChar, ScreenWidth, ScreenHeight, flags);
	if (!window)
	{
		cout << "Window creation failed: " << SDL_GetError() << endl;
		exit(1);
	}
	// creating the renderer
	renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer)
	{
		cout << "Renderer creation failed: " << SDL_GetError() << endl;
		exit(1);
	}
	// creating the OpenGL context
	OpenglContext = SDL_GL_CreateContext(window);
	if(!OpenglContext)
	{
		cout << "OpenGL context creation failed: " << SDL_GetError() << endl;
		exit(1);
	}
}