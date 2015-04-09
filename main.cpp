#include <stdexcept>
#include <iostream>

#include "texture.h"
#include "video.h"

Container _sprite_container;

int main()
{
	try {
		Video vid;

		_sprite_container.sprites.push_back(Sprite(Texture("awesomeface.png"), 50, 50));

		vid.Loop();
	} catch (const std::runtime_error &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

    return 0;
}
