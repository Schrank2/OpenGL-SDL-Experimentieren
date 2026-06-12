#include <iostream>
#include <SDL3/SDL.h>
#include "functions.h"
using namespace std;
bool debug = true;

int main(int argc, char* argv[])
{
	if (debug == true) { cout << "[DEBUG] function main from main.cpp" << endl; }
	init();
	open_window();
	while (true) {
	}
}