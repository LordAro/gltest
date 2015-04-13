#version 120

// The texture data
uniform sampler2D texture;

// Interpolated texture coordinates recieved from vertex shader
varying vec2 tex_position;

void main(void) {
	// Sets the fragment colour to the corresponding texel
	gl_FragColor = texture2D(texture, tex_position);
}
