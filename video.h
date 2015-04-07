#ifndef VIDEO_H
#define VIDEO_H

#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <SOIL/SOIL.h>


class Video {
public:
	Video();
	~Video();

	void OnResize(int w, int h);

	void Loop();
	bool EventLoop();

private:
	void Render();

	bool running;

	SDL_Window *window;
	SDL_GLContext glcontext;
};

#endif /* VIDEO_H */
