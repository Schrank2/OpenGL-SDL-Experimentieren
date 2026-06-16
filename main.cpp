#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "functions.h"
#include "defs.h"
using namespace std;
bool debug = true;

int main(int argc, char* argv[])
{
	if (debug == true) { cout << "[DEBUG] function main() from main.cpp" << endl; }
	InitSdl();
	open_window();
	//TEST
	while (true) {	
		r1.render();
	}
}