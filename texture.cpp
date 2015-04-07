#include "texture.h"

#include <SOIL/SOIL.h>

Texture::Texture(const std::string &filepath)
{
	glGenTextures(1, &this->texid);
	glBindTexture(GL_TEXTURE_2D, this->texid);

	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char *image = SOIL_load_image(filepath.c_str(), &this->width, &this->height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D); // ?

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind from current texture
}

Texture::~Texture()
{
	glDeleteTextures(1, &this->texid);
}

void Texture::OnDraw(int x, int y)
{
	glColor4f(1, 1, 1, 1);

	glActiveTexture(GL_TEXTURE0);
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
}
