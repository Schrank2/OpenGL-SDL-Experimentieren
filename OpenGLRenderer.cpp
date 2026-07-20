#include <SDL3/SDL.h>
#include "open.h"

void OpenGLRenderer::init(int* ScreenWidth, int* ScreenHeight) {
	cout << "init" << endl;
	open_window();
	create_renderer();
}

void OpenGLRenderer::render() {
	cout << "render" << endl;
}

void OpenGLRenderer::open_window() {
	cout << "open_window" << endl;
}

void OpenGLRenderer::create_renderer() {
	cout << "create_renderer" << endl;
}

OpenGLRenderer open;