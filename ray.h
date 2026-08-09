#ifndef RAY_H // only defines if variable has not been defined yet
#define RAY_H

using namespace std;
#include <iostream>
#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include "defs.h"

class RAY {
public:
	void init(float* ScreenWidth, float* ScreenHeight, float* WindowSize, string WindowTitle);
	void render();
private:
	float WindowWidthF = 0.0f;
	float WindowHeightF = 0.0f;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
};
extern RAY ray;

#endif // RAY_H