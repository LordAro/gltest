#ifndef VIDEO_H
#define VIDEO_H

#include <memory>
#include <unordered_map>

#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include "shader.h"

class Video
{
public:
	Video();
	~Video();

	void OnResize(int w, int h);

	void Loop();
	bool EventLoop();

	void AddShader(int id, Shader *vert, Shader *frag);
	ShaderProgram *GetShader(int id);

	int width;
	int height;

private:
	void Render();

	std::unordered_map<int, std::unique_ptr<ShaderProgram>> shaders;
	bool running;

	SDL_Window *window;
	SDL_GLContext glcontext;
};

extern Video _vid;

#endif /* VIDEO_H */
