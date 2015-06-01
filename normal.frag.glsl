#version 120

// The texture data
uniform sampler2D texture;
uniform sampler2D mask;

uniform int gradient_shift;
uniform vec3 recolour_rgb[2]; // arbitrary

// Interpolated texture coordinates recieved from vertex shader
varying vec2 tex_position;

const float STEP_SIZE = 18 / 255.0;

vec3 rgb2yuv(vec3 val)
{
	return vec3( 0.299 * val.r + 0.587 * val.g + 0.114  * val.b +   0,
	            -0.169 * val.r - 0.331 * val.g + 0.499  * val.b + 128,
	             0.499 * val.r - 0.418 * val.g - 0.0813 * val.b + 128);
}

vec3 yuv2rgb(vec3 val)
{
	return clamp(vec3(val.x + 1.402 * (val.z - 128),
	                  val.x - 0.344 * (val.y - 128) - 0.714 * (val.z - 128),
	                  val.x + 1.772 * (val.y - 128)
	                 ), vec3(0), vec3(255));
}

// fetch image texel
// fetch mask
// if recolourised
//     fetch palette texel with base colour
//     calc Y from original image
//     multiply by base colour
// gradient shift
void main(void) {
	vec4 col = texture2D(texture, tex_position);
	// Unfloat mask value
	int mask_col = int(floor(texture2D(mask, tex_position).r * 255.0 + 0.5));
	if (mask_col != 0) {
		// something
		gl_FragColor = vec4(recolour_rgb[mask_col - 1], 1.0);
		return;
	}

	// Gradient shifting
	vec3 new_rgb = clamp(col.rgb + vec3(gradient_shift * STEP_SIZE), vec3(0.0), vec3(1.0));
	gl_FragColor = vec4(new_rgb, col.a);
}
