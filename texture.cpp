#include <stdexcept>

#include <SDL2/SDL_image.h>

#include "texture.h"
#include "video.h"

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

	glGenBuffers(1, &this->vbo);
}

Texture::~Texture()
{
	glDeleteTextures(1, &this->texid);
	glDeleteBuffers(1, &this->vbo);
}

void Texture::OnDraw(int x, int y) const
{
	glColor4f(1, 1, 1, 1); // Should be black?

	auto shader = _vid.GetShader(0);

	// Update parameters
	glUniform1i(shader->grad_shift_id, 0);

	shader->Use();
	int *pos_id = &shader->pos_id;
	int *texcoord_id = &shader->tex_coord;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texid);

	float left = x;
	float right = x + this->width;
	float top = y;
	float bottom = y + this->height;

	float vertices[] = {
		left,  top,    0, 0,
		right, top,    1, 0,
		left,  bottom, 0, 1,
		right, top,    1, 0,
		left,  bottom, 0, 1,
		right, bottom, 1, 1,
	};

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(*pos_id);
	glEnableVertexAttribArray(*texcoord_id);
	glVertexAttribPointer(*pos_id, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(0));
	glVertexAttribPointer(*texcoord_id, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2*sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, 6); // 2 triangles

	/* Clean up */
	glDisableVertexAttribArray(*pos_id);
	glDisableVertexAttribArray(*texcoord_id);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	shader->StopUsing();
}
