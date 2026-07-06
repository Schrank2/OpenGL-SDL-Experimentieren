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
vector<string> Report;
int bpp = 0;
int flags = 0;
SDL_Window* window;
SDL_Renderer* renderer;
SDL_GLContext OpenglContext;
string WindowTitle;


void init_libs()	{
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
	// Initialize SDL_ttf library and check for errors.
	if (TTF_Init() == 0)
	{
		cout << "SDL_ttf initialization failed: " << SDL_GetError() << endl;
		exit(1);
	}

	// Display Versions of Libraries
	cout << "SDL Version: " << SDL_GetVersion() << endl;
	cout << "SDL_ttf Version: " << TTF_Version() << endl;
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

RGBA_int FloatToIntColor(RGBA_float color) {
	return RGBA_int(static_cast<int>(color.r), static_cast<int>(color.g), static_cast<int>(color.b), static_cast<int>(color.a));
}

float CapColor_float(float color) {
	if (color < 0.0f) { return 0.0f; }
	if (color > 255.0f) { return 255.0f; }
	return color;
}

int CapColor_int(int color) {
	if (color < 0) { return 0; }
	if (color > 255) { return 255; }
	return static_cast<int>(color);
}