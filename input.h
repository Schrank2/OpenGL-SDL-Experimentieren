#ifndef INPUT_H // only defines if variable has not been defined yet
#define INPUT_H

using namespace std;
#include <iostream>
#include <SDL3/SDL.h>
#include <vector>

struct InputMK {
	bool esc = false;
	int esc_LastTime = 0;
	bool w = false;
	bool a = false;
	bool s = false;
	bool d = false;
	bool space = false;
	bool lshift = false;
	bool p = false;
	bool o = false;
	int o_LastTime = 0;
	bool f3 = false;
	int f3_LastTime = 0;
	bool f3_lastState = false;
};

struct Button {
	bool active = false;
	bool pressed = false;
	SDL_Keycode KeyCode;
	// "Toggle" special behaviour
	bool isToggle = false;
	bool currentToggle = false;
	// "Delay" special behaviour
	bool hasDelay = false; 
	int Delay = 0;
	int lastPress = 0;
	Button(SDL_Keycode KeyCode, bool isToggle, bool hasDelay, bool Delay) : KeyCode(KeyCode), isToggle(isToggle), hasDelay(hasDelay), Delay(Delay) {};
};

struct Mouse {
	float MouseX = 0.0f;
	float MouseY = 0.0f;
	float LastMouseX = 0.0f;
	float LastMouseY = 0.0f;
	float MouseMovementX = 0.0f;
	float MouseMovementY = 0.0f;
	float MouseSensitivity = 10.0f;
};

class INPUTCLASS {
	public:
		void poll(vector<Button>* Input);
		void pollButton(Button* Button, int tick);
		void init(vector<Button>* Input);
};

extern INPUTCLASS input;

#endif // INPUT_H