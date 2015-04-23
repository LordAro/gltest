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

	int width;
	int height;

private:
	GLuint texid;
	GLuint vbo;
};

class VoxelObject {
public:
	VoxelObject(Texture *tex, int x, int y) : tex(tex), x(x), y(y) {}
	Texture* tex;
	int x, y;
};

class Container {
public:
	std::vector<VoxelObject> objects;

	void OnDraw()
	{
		for (const auto &obj : this->objects) {
			obj.tex->OnDraw(obj.x, obj.y);
		}
	}
};

extern Container _sprite_container;


#endif /* TEXTURE_H */
