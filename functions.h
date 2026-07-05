#ifndef FUNCTIONS_H // only defines if variable has not been defined yet
#define FUNCTIONS_H

#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "defs.h"
using namespace std;
void init_libs();
void open_window();
RGBA_int FloatToIntColor(RGBA_float color);

#endif // FUNCTIONS_H