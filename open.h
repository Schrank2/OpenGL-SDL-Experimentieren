#ifndef OPEN_H // only defines if variable has not been defined yet
#define OPEN_H

using namespace std;
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <string>
#include <vector>
#include "defs.h"

// declare OpenGLRenderer so open.cpp can use it
class OpenGLRenderer {
public:
	void init(int* ScreenWidth, int* ScreenHeight);
	void render();
private:
	void open_window();
	void create_renderer();
};
// declare r1 so the R1 functions can be called elsewhere
extern OpenGLRenderer open;

#endif // OPEN_H