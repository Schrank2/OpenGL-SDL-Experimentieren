#include <SDL3/SDL.h>
#include "input.h"
using namespace std;

InputMK Input;
SDL_Event event;

void INPUTCLASS::pollInput(InputMK* Input) {
	cout << "TEST" << endl;
	if (SDL_PollEvent(&event) && event.type == SDL_EVENT_KEY_DOWN) {
		if (event.key.key == SDLK_ESCAPE) {
			Input.esc = true;
		}
		if (event.key.key == SDLK_W) {
			Input.w = true;
		}
		if (event.key.key == SDLK_A) {
			Input.a = true;
		}
		if (event.key.key == SDLK_S) {
			Input.s = true;
		}
		if (event.key.key == SDLK_D) {
			Input.d = true;
		}
		if (event.key.key == SDLK_SPACE) {
			Input.space = true;
		}
		if (event.key.key == SDLK_LSHIFT) {
			Input.lshift = true;
		}
		if (event.key.key == SDLK_P) {
			Input.p = true;
		}
		if (event.key.key == SDLK_O) {
			Input.o = true;
		}
		if (event.key.key == SDLK_F3) {
			Input.f3 = true;
		}
	}
	if (event.type == SDL_EVENT_KEY_UP) {
		if (event.key.key == SDLK_ESCAPE) {
			Input.esc = false;
		}
		if (event.key.key == SDLK_W) {
			Input.w = false;
		}
		if (event.key.key == SDLK_A) {
			Input.a = false;
		}
		if (event.key.key == SDLK_S) {
			Input.s = false;
		}
		if (event.key.key == SDLK_D) {
			Input.d = false;
		}
		if (event.key.key == SDLK_SPACE) {
			Input.space = false;
		}
		if (event.key.key == SDLK_LSHIFT) {
			Input.lshift = false;
		}
		if (event.key.key == SDLK_P) {
			Input.p = false;
		}
		if (event.key.key == SDLK_O) {
			Input.o = false;
		}
		if (event.key.key == SDLK_F3) {
			Input.f3 = false;
		}
	}
}

INPUTCLASS input;