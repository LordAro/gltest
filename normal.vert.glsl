#version 120

// Modelview*projection matrix
uniform mat4 mvp_matrix;

// Position of this vertex
attribute vec4 vertex_position;

// Coordinates assigned to this vertex
attribute vec2 tex_coords;

// Interpolated texture coordinates sent to fragment shader
varying vec2 tex_position;

void main(void) {
	// Transform the vertex coordinates
	gl_Position = gl_ModelViewProjectionMatrix * vertex_position;

	// Pass the fix points for texture coordinates set at this vertex
	tex_position = tex_coords;
}
