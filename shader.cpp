#include <cassert>
#include <stdexcept>
#include <string>

#include "shader.h"

Shader::Shader(GLenum type, const char *source) : type(type)
{
	this->id = glCreateShader(type);

	// Load & compile
	glShaderSource(this->id, 1, &source, nullptr);
	glCompileShader(this->id);

	// Get compile result
	GLint status;
	glGetShaderiv(this->id, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) {
		GLint loglen;
		glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &loglen);

		char *infolog = new char[loglen];
		glGetShaderInfoLog(this->id, loglen, nullptr, infolog);

		std::string err = "Failed to compile ";
		err += Shader::TypeToString(this->type);
		err += " shader:\n";
		err += infolog;
		delete infolog;
		throw std::runtime_error(err);
	}
}

Shader::~Shader()
{
	glDeleteShader(this->id);
}

/* static */ const char *Shader::TypeToString(GLenum type)
{
	switch (type) {
		case GL_VERTEX_SHADER:
			return "vertex";
		case GL_FRAGMENT_SHADER:
			return "fragment";
		case GL_GEOMETRY_SHADER:
			return "geometry";
		default:
			throw std::runtime_error("Unknown shader type");
	}
}

ShaderProgram::ShaderProgram(Shader *vert, Shader *frag)
{
	this->id = glCreateProgram();

	glAttachShader(this->id, vert->id);
	glAttachShader(this->id, frag->id);

	glLinkProgram(this->id);
	GLint status;
	glGetProgramiv(this->id, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		// TODO: More debug info
		throw std::runtime_error("Program failed to link");
	}

	glValidateProgram(this->id);
	glGetProgramiv(this->id, GL_VALIDATE_STATUS, &status);
	if (status != GL_TRUE) {
		// TODO: More debug info
		throw std::runtime_error("Program failed to validate");
	}

	this->pos_id = glGetAttribLocation(this->id, "vertex_position");

	glDetachShader(this->id, vert->id);
	glDetachShader(this->id, frag->id);

	this->texture = glGetUniformLocation(this->id, "texture");
	this->mask = glGetUniformLocation(this->id, "mask");

	this->grad_shift_id = glGetUniformLocation(this->id, "gradient_shift");
	this->recolour = glGetUniformLocation(this->id, "recolour_rgb");

	this->tex_coord = glGetAttribLocation(this->id, "tex_coords");

	// Need to use the shader to set uniforms
	this->Use();
	glUniform1i(this->texture, 0);
	glUniform1i(this->mask, 1);
	this->StopUsing();
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(this->id);
}

void ShaderProgram::Use()
{
	glUseProgram(this->id);
}

void ShaderProgram::StopUsing()
{
	glUseProgram(0);
}
