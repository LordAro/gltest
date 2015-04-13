#ifndef TEXTURE_H
#define TEXTURE_H

#include <memory>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>

class Texture {
public:
	Texture(const std::string &filepath);
	Texture(const Texture&) = delete;

	~Texture();

	void OnDraw(int x, int y) const;

private:
	GLuint texid;
	GLuint vbo;
	int width;
	int height;
};

class Sprite {
public:
	Sprite(Texture *tex, int x, int y) : tex(tex), x(x), y(y) {}
	std::unique_ptr<Texture> tex;
	int x, y;
};

class Container {
public:
	std::vector<Sprite> sprites;

	void OnDraw()
	{
		for (const auto &sprite : this->sprites) {
			sprite.tex->OnDraw(sprite.x, sprite.y);
		}
	}
};

extern Container _sprite_container;


#endif /* TEXTURE_H */
