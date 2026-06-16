#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "functions.h"
using namespace std;
bool debug = true;

int main(int argc, char* argv[])
{
	if (debug == true) { cout << "[DEBUG] function main from main.cpp" << endl; }
	init();
	open_window();
	//TEST
	while (true) {	
		R1.render();
	}
}