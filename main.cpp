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

		auto tile = std::unique_ptr<Texture>(new Texture("tile.png"));

		for (int x = -64; x + tile->width < _vid.width; x+=64) {
			for (int y = -64; y + tile->height < _vid.height; y+=16) {
				int x1 = x;
				if (y % 32 == 0) x1 -= 32;
				_sprite_container.objects.emplace_back(tile.get(), x1, y);
			}
		}

		auto icecream = std::unique_ptr<Texture>(new Texture("icecream.png", "icecream_mask.png"));
		_sprite_container.objects.emplace_back(icecream.get(), 544, 256);

		_vid.Loop();
	} catch (const std::runtime_error &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

    return 0;
}
