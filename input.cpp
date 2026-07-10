#include <SDL3/SDL.h>
#include "input.h"
using namespace std;

SDL_Event InputEvent;

void INPUTCLASS::poll(vector<Button>* Input) {
	int tick = SDL_GetTicks();
	int i = 0;
	while (SDL_PollEvent(&InputEvent)) {
		for (Button& button : *Input) {
			pollButton(&button, &InputEvent, tick);
		}
	}
}

void INPUTCLASS::init(vector<Button>* Input) {
	Input->push_back(Button(SDLK_W, false, false, 0));
	Input->push_back(Button(SDLK_A, false, false, 0));
	Input->push_back(Button(SDLK_S, false, false, 0));
	Input->push_back(Button(SDLK_D, false, false, 0));
	Input->push_back(Button(SDLK_SPACE, false, false, 0));
	Input->push_back(Button(SDLK_LSHIFT, false, false, 0));
	Input->push_back(Button(SDLK_ESCAPE, true, false, 0));
	Input->push_back(Button(SDLK_O, true, false, 0));
	Input->push_back(Button(SDLK_F3, true, false, 0));
}

void INPUTCLASS::pollButton(Button* Button, SDL_Event* event, int tick ) {
	if (Button->hasDelay == true && tick < tick + Button->Delay) {
		return;
	}
	Button->lastPress = tick;

	if (event->type == SDL_EVENT_KEY_UP) {
		if (event->key.key == Button->KeyCode) {
			Button->pressed = false;
		}
	}
	if (event->type == SDL_EVENT_KEY_DOWN) {
		if (event->key.key == Button->KeyCode) {
			Button->pressed = true;
		}
	}
	if (Button->isToggle == false) {
		Button->active = Button->pressed ? true : false;
		return;
	}

	if (Button->pressed == true) {
		if (Button->currentToggle == false) {
			Button->currentToggle = true;
			Button->active = Button->active ? false : true;
		}
	}
	if (Button->pressed == false) {
		if (Button->currentToggle == true) {
			Button->currentToggle = false;
		}
	}
}

void INPUTCLASS::pollMouse(Mouse* Mouse) {
	SDL_GetRelativeMouseState(&Mouse->x, &Mouse->y);
	Mouse->movex += Mouse->x - Mouse->lastx;
	Mouse->movey += Mouse->y - Mouse->lasty;
	Mouse->lastx = Mouse->x;
	Mouse->lasty = Mouse->y;
}


INPUTCLASS input;