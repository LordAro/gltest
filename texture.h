#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

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


#endif /* TEXTURE_H */
