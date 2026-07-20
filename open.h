#ifndef OPEN_H // only defines if variable has not been defined yet
#define OPEN_H

using namespace std;
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

// declare OpenGLRenderer so open.cpp can use it
class OpenGLRenderer {
public:
	void init(int* ScreenWidth, int* ScreenHeight);
	void render();
	void CleanUp();
	int WindowHeight = 0;
	int WindowWidth = 0;
	SDL_Window* Window = nullptr;
	SDL_GLContext GLContext = nullptr;
private:
	void open_window();
	void create_renderer();
	void create_GLContext(SDL_Window* Window);
	void Input();
	void preDraw();
	void Draw();
};
// declare r1 so the R1 functions can be called elsewhere
extern OpenGLRenderer open;

#endif // OPEN_H