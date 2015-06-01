#include <iostream>
#include <stdexcept>

#include <SDL2/SDL_image.h>

#include "texture.h"
#include "video.h"

Texture::Texture(const std::string &img_path, const std::string &mask_path)
{
	this->texid = Texture::MakeGLTexture(img_path, &this->width, &this->height);
	if (!mask_path.empty()) {
		// TODO: Care about different widths and heights of masks.
		this->maskid = Texture::MakeGLTexture(mask_path, &this->width, &this->height);
	}

	glGenBuffers(1, &this->vbo);
}

Texture::~Texture()
{
	glDeleteTextures(1, &this->texid);
	glDeleteTextures(1, &this->maskid); // Does nothing if not set
	glDeleteBuffers(1, &this->vbo);
}

void Texture::OnDraw(int x, int y) const
{
	glColor4f(1, 1, 1, 1); // Should be black?

	auto shader = _vid.GetShader(0);
	shader->Use();

	// Update parameters
	glUniform1i(shader->grad_shift_id, 0);
	float recols[] = {
		1.0, 0.0, 0.0,
		0.0, 0.0, 1.0,
	};
	glUniform3fv(shader->recolour, 2, recols);

	int *pos_id = &shader->pos_id;
	int *texcoord_id = &shader->tex_coord;

	if (this->HasMask()) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->maskid);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texid);

	float left = x;
	float right = x + this->width;
	float top = y;
	float bottom = y + this->height;

	float vertices[] = {
	        left,  top,    0, 0, // top-left
	        right, top,    1, 0, // top-right
	        left,  bottom, 0, 1, // bottom-left
	        right, top,    1, 0, // top-right
	        left,  bottom, 0, 1, // bottom-left
	        right, bottom, 1, 1, // bottom-right
	};

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(*pos_id);
	glEnableVertexAttribArray(*texcoord_id);
	glVertexAttribPointer(*pos_id,      2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(0));
	glVertexAttribPointer(*texcoord_id, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, 6); // 2 triangles

	/* Clean up */
	glDisableVertexAttribArray(*pos_id);
	glDisableVertexAttribArray(*texcoord_id);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	shader->StopUsing();
}

/* static */ GLuint Texture::MakeGLTexture(const std::string &filepath, int *width, int *height)
{
	GLuint texture;

	glGenTextures(1, &texture);
	if (texture == 0) {
		std::string err = "Error creating texture: ";
		err += (const char *)gluErrorString(glGetError());
		throw std::runtime_error(err);
	}
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// TODO: Check filepath
	SDL_Surface *surface = IMG_Load(filepath.c_str());
	GLint int_format;
	GLenum format;
	switch (surface->format->BytesPerPixel) {
		case 1: // uint8 (paletted mask sprite)
			int_format = GL_RGBA8;
			format = GL_LUMINANCE;
			break;
		case 4: // uint32
			int_format = GL_RGBA8;
			format = GL_RGBA;
			break;
		default:
			throw std::runtime_error("Unknown texture bit depth for " + filepath + " (" +
			                         std::to_string(surface->format->BytesPerPixel) + ")");
	}
	*width = surface->w;
	*height = surface->h;

	glTexImage2D(GL_TEXTURE_2D, 0, int_format, *width, *height, 0, format, GL_UNSIGNED_BYTE, surface->pixels);

	SDL_FreeSurface(surface);

	return texture;
}
