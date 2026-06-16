#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "functions.h"
#include "defs.h"
#include <vector>

void GameLogic::init() {
	if (debug == true) { cout << "[DEBUG] function game.init() from game.cpp" << endl; }
}

void GameLogic::tick() {
	if (debug == true) { cout << "[DEBUG] function game.tick() from game.cpp" << endl; }
}

GameLogic game;