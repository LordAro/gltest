#ifndef TEXTURE_H
#define TEXTURE_H

#include <memory>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>

class Texture
{
public:
	Texture(const std::string &img_path, const std::string &mask_path);
	Texture(const std::string &img_path) : Texture(img_path, std::string())
	{
	}
	Texture(const Texture &) = delete;

	~Texture();

	void OnDraw(int x, int y) const;

	int width;
	int height;

private:
	static GLuint MakeGLTexture(const std::string &filepath, int *width, int *height);
	inline bool HasMask() const
	{
		return this->maskid != 0;
	}

	GLuint texid;
	GLuint maskid;
	GLuint vbo;
};

class VoxelObject
{
public:
	VoxelObject(Texture *tex, int x, int y) : tex(tex), x(x), y(y)
	{
	}
	Texture *tex;
	int x, y;
};

class Container
{
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
