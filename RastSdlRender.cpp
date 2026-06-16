#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "functions.h"
#include "defs.h"
#include <thread>
#include <vector>
#include <string>
#include <functional>

void R1::render() {
	if (debug == true) { cout << "[DEBUG] function render from rastSdlRender.cpp" << endl; }
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}