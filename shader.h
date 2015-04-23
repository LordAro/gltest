#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/gl.h>

class Shader
{
public:
	Shader(GLenum type, const char *source);
	~Shader();

	GLuint id;
	GLenum type;

private:
	static const char *TypeToString(GLenum type);
};

class ShaderProgram
{
public:
	ShaderProgram(Shader *vert, Shader *frag);
	~ShaderProgram();

	void Use();
	void StopUsing();

	GLint tex_coord;
	GLint pos_id;
	GLint mvpm_id;
	GLint grad_shift_id;
	GLint texture;

private:
	GLuint id;
};

#endif /* SHADER_H */
