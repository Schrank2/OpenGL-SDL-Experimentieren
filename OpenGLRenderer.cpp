#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <iostream>
#include "OpenGLRenderer.h"
#include <vector>

GLuint gVertexArrayObject = 0;
GLuint gVertexBufferObject = 0;
GLuint gShaderPipeline = 0;

const std::string gVertexShaderSource =
"#version 410 core\n"
"in vec4 position; \n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, position.w);\n"
"}\n";
const std::string gFragmentShaderSource =
"#version 410 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.0f, 1.0f);\n"
"}\n";

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
	CreateGraphicsPipeline();
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

GLuint OpenGLRenderer::CompileShader(GLuint type, const std::string& source) {
	GLuint shaderObject;

	if (type == GL_VERTEX_SHADER) {
		shaderObject = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (type == GL_FRAGMENT_SHADER) {
		shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	}
	const GLchar* sourceChar = source.c_str();
	glShaderSource(shaderObject, 1, &sourceChar, nullptr);
	glCompileShader(shaderObject);
	return shaderObject;
}

GLuint OpenGLRenderer::CreateShaderProgram(const std::string& VertexShaderSource, const std::string& FragmentShaderSource) {
	GLuint programObject = glCreateProgram();
	GLuint VertexShader = CompileShader(GL_VERTEX_SHADER, VertexShaderSource);
	GLuint FragmentShader = CompileShader(GL_FRAGMENT_SHADER, FragmentShaderSource);
	glAttachShader(programObject, VertexShader);
	glAttachShader(programObject, FragmentShader);
	glLinkProgram(programObject);
	// validate our program
	glValidateProgram(programObject);
	return programObject;
}

void OpenGLRenderer::CreateGraphicsPipeline() {
	gShaderPipeline = CreateShaderProgram(gVertexShaderSource, gFragmentShaderSource);
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
	cout << "preDraw" << endl;
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0, 0, WindowWidth, WindowHeight);
	glClearColor(1.0f, 1.0f, 0.1f, 1.0f);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram(gShaderPipeline);
}

void OpenGLRenderer::Draw() {
	glBindVertexArray(gVertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

OpenGLRenderer open;