#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <iostream>
#include "open.h"

void OpenGLRenderer::init(int* ScreenWidth, int* ScreenHeight) {
	std::cout << "init" << endl;
	WindowHeight = *ScreenHeight;
	WindowWidth = *ScreenWidth;
	open_window();
	create_GLContext(Window);
	create_renderer();
}

void OpenGLRenderer::render() {
	std::cout << "render" << endl;
}

void OpenGLRenderer::open_window() {
	std::cout << "open_window" << endl;
	Window = SDL_CreateWindow("OpenGL Window", WindowWidth, WindowHeight, SDL_WINDOW_OPENGL);
	if (!Window) {
		std::cout << "open.open_window was unsuccessful in creating the Window" << endl;
		exit(1);
	}
}

void OpenGLRenderer::create_GLContext(SDL_Window* Window) {
	std::cout << "create_GLContext" << endl;
	GLContext = SDL_GL_CreateContext(Window);
	if (!GLContext) {
		std::cout << "open.create_GLContext was unsuccessful in creating the OpenGL Context" << endl;
		exit(1);
	}
}

void OpenGLRenderer::create_renderer() {
	std::cout << "create_renderer" << endl;
}

OpenGLRenderer open;