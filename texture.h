#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>

class Texture {
public:
	Texture(const std::string &filepath);

	~Texture();

	void OnDraw(int x, int y);

private:
	GLuint texid;
	int width;
	int height;
};

class Sprite {
public:
	Sprite(Texture tex, int x, int y) : tex(tex), x(x), y(y) {}
	Texture tex;
	int x, y;
};

class Container {
public:
	std::vector<Sprite> sprites;

	void OnDraw()
	{
		for (auto sprite : this->sprites) {
			sprite.tex.OnDraw(sprite.x, sprite.y);
		}
	}
};

extern Container _sprite_container;


#endif /* TEXTURE_H */
