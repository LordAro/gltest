#include <stdexcept>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <GL/gl.h>

#include "video.h"
#include "texture.h"

Video::Video() : running(true)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::string err = "Error initialising video: ";
		err += SDL_GetError();
		throw std::runtime_error(err);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED;

	this->window = SDL_CreateWindow("lolpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, flags);
	if (this->window == nullptr) {
		std::string err = "Error creating SDL window: ";
		err += SDL_GetError();
		throw std::runtime_error(err);
	}

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		std::string err = "Error initialising SDL_image: ";
		err += IMG_GetError();
		throw std::runtime_error(err);
	}

	this->glcontext = SDL_GL_CreateContext(this->window);
	if (this->glcontext == nullptr) {
		std::string err = "Error creating GL context: ";
		err += SDL_GetError();
		throw std::runtime_error(err);
	}

	GLenum glew_err = glewInit();
	if (glew_err != GLEW_OK) {
		std::string err = "Error initialising GLEW: ";
		err += (char *)glewGetErrorString(glew_err);
		throw std::runtime_error(err);
	}

	if (!GLEW_VERSION_2_1) {
		throw std::runtime_error("OpenGL 2.1 not available");
	}

	int max_tex_size;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);
	if (max_tex_size < 1024) {
		throw std::runtime_error("Maximum supported texture size too small");
	}

	int max_tex_units;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_tex_units);
	if (max_tex_units < 2) {
		throw std::runtime_error("GPU does not have enough texture unnits");
	}

	SDL_GL_SetSwapInterval(1); // vsync

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DEPTH_TEST); // what's drawn last is displayed on top.
	glEnable(GL_TEXTURE_2D);
}

Video::~Video()
{
	SDL_GL_DeleteContext(this->glcontext);
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}

void Video::OnResize(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);

	glOrtho(0, w, h, 0, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Video::Loop()
{
	while (this->running) {
		this->running = this->EventLoop();
		if (!this->running) break;

		this->Render();
	}
}

bool Video::EventLoop()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT:
				return false;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					return false;
				}
				break;
			case SDL_WINDOWEVENT:
				if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
					this->OnResize(e.window.data1, e.window.data2);
				}
			default:
				break;
		}
	}
	return true;
}

void Video::Render()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	{
		_sprite_container.OnDraw();
	}
	glPopMatrix();


	SDL_GL_SwapWindow(this->window);
}
