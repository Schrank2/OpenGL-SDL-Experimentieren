#ifndef OPEN_H // only defines if variable has not been defined yet
#define OPEN_H

using namespace std;
#include <glad/glad.h>
#include <iostream>
#include <SDL3/SDL.h>
//#include <SDL3/SDL_opengl.h>

// declare OpenGLRenderer so open.cpp can use it
class OpenGLRenderer {
public:
	void init(int* ScreenWidth, int* ScreenHeight);
	void render();
	void CleanUp();
	int WindowHeight = 0;
	int WindowWidth = 0;
	SDL_Window* Window = nullptr;
	SDL_GLContext GLContext = nullptr;
	bool active = true;
private:
	void open_window();
	void create_GLContext(SDL_Window* Window);
	void Input();
	void preDraw();
	void Draw();
	string LoadShaderFile(const std::string& directory);
	GLuint CompileShader(GLuint type, const std::string& source);
	void CreateGraphicsPipeline();
	GLuint CreateShaderProgram(const std::string& VertexShaderSource, const std::string& FragmentShaderSource);
};
// declare r1 so the R1 functions can be called elsewhere
extern OpenGLRenderer open;

#endif // OPEN_H