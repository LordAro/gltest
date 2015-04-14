#version 120

// The texture data
uniform sampler2D texture;

uniform int gradient_shift;

// Interpolated texture coordinates recieved from vertex shader
varying vec2 tex_position;

const float STEP_SIZE = 18 / 255.0;

void main(void) {
	vec4 col = texture2D(texture, tex_position);
	vec3 new_rgb = max(min(col.rgb + vec3(gradient_shift * STEP_SIZE), vec3(1.0)), vec3(0.0));
	gl_FragColor = vec4(new_rgb, col.a);
}
