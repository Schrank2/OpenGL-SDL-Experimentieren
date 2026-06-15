using namespace std;
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
/* Information about the current video settings. */
/* extern const SDL_VideoInfo; */
/* Dimensions of our window. */
extern int ScreenWidth;
extern int ScreenHeight;
/* Color depth in bits of our window. */
extern int bpp;
/* Flags we will pass into SDL_SetVideoMode. */
extern int flags;
extern bool debug;
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_GLContext OpenglContext;

