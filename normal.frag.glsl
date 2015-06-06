#version 120

// The texture data
uniform sampler2D texture;
uniform sampler2D mask;

uniform int gradient_shift;
uniform vec3 recolour_rgb[2]; // arbitrary

// Interpolated texture coordinates recieved from vertex shader
varying vec2 tex_position;

const float STEP_SIZE = 18 / 255.0;
const vec3 RGB_TO_LUMA = vec3(0.299, 0.587, 0.114);

float RGBToLuma(vec3 rgb) {
	return dot(rgb, RGB_TO_LUMA);
}

void main(void) {
	// fetch image texel
	vec4 col = texture2D(texture, tex_position);

	// Fetch mask & unfloat value
	int mask_col = int(floor(texture2D(mask, tex_position).r * 255.0 + 0.5));
	if (mask_col != 0) {
		vec3 recol = recolour_rgb[mask_col - 1]; // Target colour
		// calc Y' from original image & multiply by base colour
		float orig_luma = RGBToLuma(col.rgb);
		vec3 converted = orig_luma * recol;
		converted *= RGBToLuma(converted) - orig_luma;
		col = vec4(converted, col.a);
	}

	// Gradient shifting
	vec3 new_rgb = clamp(col.rgb + vec3(gradient_shift * STEP_SIZE), vec3(0.0), vec3(1.0));
	gl_FragColor = vec4(new_rgb, col.a);
}
