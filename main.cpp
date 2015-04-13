#include <fstream>
#include <iostream>
#include <stdexcept>

#include "texture.h"
#include "video.h"

Container _sprite_container;
Video _vid;

std::string LoadFile(const char *filepath)
{
	std::ifstream f(filepath);
	std::string str;

	f.seekg(0, std::ios::end);
	str.reserve(f.tellg());
	f.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
	return str;
}

int main()
{
	try {

		std::string vert_src = LoadFile("normal.vert.glsl");
		std::string frag_src = LoadFile("normal.frag.glsl");
		Shader *vert = new Shader(GL_VERTEX_SHADER, vert_src.c_str());
		Shader *frag = new Shader(GL_FRAGMENT_SHADER, frag_src.c_str());

		_vid.AddShader(0, vert, frag);
		delete vert;
		delete frag;

		_sprite_container.sprites.push_back(Sprite(new Texture("awesomeface.png"), 50, 50));

		_vid.Loop();
	} catch (const std::runtime_error &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

    return 0;
}
