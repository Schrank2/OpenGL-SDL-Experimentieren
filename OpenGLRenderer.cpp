#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <iostream>
#include "OpenGLRenderer.h"
#include <vector>

GLuint gVertexArrayObject = 0;
GLuint gVertexBufferObject = 0;

void OpenGLRenderer::init(int* ScreenWidth, int* ScreenHeight) {
	WindowHeight = *ScreenHeight;
	WindowWidth = *ScreenWidth;
	// OPENGL Attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	open_window();
	create_GLContext(Window);

	// Initialise glad (importantly, after Context Creation
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::cout << "[OPENGLRENDERER] glad was not initialized" << std::endl;
		exit(1);
	}
	std::cout << "[OPENGLRENDERER] OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
}

void OpenGLRenderer::open_window() {
	Window = SDL_CreateWindow("OpenGL Window", WindowWidth, WindowHeight, SDL_WINDOW_OPENGL);
	if (!Window) {
		std::cout << "[OPENGLRENDERER] open.open_window was unsuccessful in creating the Window" << endl;
		exit(1);
	}
}

void OpenGLRenderer::create_GLContext(SDL_Window* Window) {
	GLContext = SDL_GL_CreateContext(Window);
	if (!GLContext) {
		std::cout << "[OPENGLRENDERER] open.create_GLContext was unsuccessful in creating the OpenGL Context" << endl;
		exit(1);
	}
}

void OpenGLRenderer::CleanUp() {
	SDL_DestroyWindow(Window);
}

void OpenGLRenderer::CreateGraphicsPipeline() {

}


void OpenGLRenderer::render() {
	Input();

	preDraw();

	Draw();

	SDL_GL_SwapWindow(Window);
}

void OpenGLRenderer::Input() {
	// create vertex data on the CPU
	const std::vector<GLfloat> vertexPosition{
		// x y z
		-0.8f, -0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
		0.0f, 0.8f, 0.0f
	};
	// bind vertex Data to the GPU
	glGenVertexArrays(1, &gVertexArrayObject);
	glBindVertexArray(gVertexArrayObject);

	glGenBuffers(1, &gVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER,
				 vertexPosition.size() * sizeof(GLfloat),
				 vertexPosition.data(),
				 GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*) 0);
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
}

void OpenGLRenderer::preDraw() {

}

void OpenGLRenderer::Draw() {

}

OpenGLRenderer open;