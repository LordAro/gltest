#include <stdexcept>

#include <SDL2/SDL_image.h>

#include "texture.h"


Texture::Texture(const std::string &filepath)
{
	glGenTextures(1, &this->texid);
	if (this->texid == 0) {
		const char *glerr = (const char *)gluErrorString(glGetError());
		std::string err = "Error creating texture: ";
		err += glerr;
		throw std::runtime_error(err);
	}
	glBindTexture(GL_TEXTURE_2D, this->texid);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_Surface *surface = IMG_Load(filepath.c_str());
	GLint int_format;
	GLenum format;
	switch (surface->format->BytesPerPixel) {
		case 3:
			int_format = GL_RGB8;
			format = GL_RGB;
			break;
		case 4:
			int_format = GL_RGBA8;
			format = GL_RGBA;
			break;
		default:
			throw std::runtime_error("unknown texture bit depth for" + filepath);
	}
	this->width = surface->w;
	this->height = surface->h;
	glTexImage2D(GL_TEXTURE_2D, 0, int_format, this->width, this->height, 0, format, GL_UNSIGNED_BYTE, surface->pixels);

	SDL_FreeSurface(surface);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind from current texture
}

Texture::~Texture()
{
	glDeleteTextures(1, &this->texid);
}

void Texture::OnDraw(int x, int y)
{
	glColor4f(1, 1, 1, 1); // Black?

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->texid);

	glBegin(GL_TRIANGLES);
	{
		glTexCoord2f(0, 0); glVertex3f(x, y, 0);
		glTexCoord2f(1, 0); glVertex3f(x + this->width, y, 0);
		glTexCoord2f(0, 1); glVertex3f(x, y + this->height, 0);

		glTexCoord2f(1, 0); glVertex3f(x + this->width, y, 0);
		glTexCoord2f(0, 1); glVertex3f(x, y + this->height, 0);
		glTexCoord2f(1, 1); glVertex3f(x + this->width, y + this->height, 0);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}
