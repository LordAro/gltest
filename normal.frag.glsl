#version 120

// The texture data
uniform sampler2D texture;

// Interpolated texture coordinates recieved from vertex shader
varying vec2 tex_position;

const float STEP_SIZE = 18 / 255.0;

void main(void) {
	// Sets the fragment colour to the corresponding texel
	vec4 col = texture2D(texture, tex_position);
	gl_FragColor = max(col - vec4(4.0 * STEP_SIZE), vec4(0.0));
}
