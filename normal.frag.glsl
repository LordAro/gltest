#version 120

// The texture data
uniform sampler2D texture;
uniform sampler2D mask;

uniform int gradient_shift;
uniform vec3 recolour_rgb[2]; // arbitrary

// Interpolated texture coordinates recieved from vertex shader
varying vec2 tex_position;

const float STEP_SIZE = 18 / 255.0;

// numbers and formulas acquired from http://www.poynton.com/PDFs/ColorFAQ.pdf
const mat3 YCbCr_CONVERT = mat3( 65.481, 128.553,  24.966,
                                -37.797, -74.203, 112.000,
                                112.000, -93.786, -18.214);
// [0, 1] -> [0, 255]
vec3 RGB2YCbCr(vec3 val)
{
	vec3 tmp = YCbCr_CONVERT * val;
	tmp += vec3(16, 128, 128);
	return clamp(tmp, 1, 254);
}

const mat3 RGB_CONVERT   = mat3(0.00456621,  0.0,         0.00625893,
                                0.00456621, -0.00153632, -0.00318811,
                                0.00456621,  0.00791071,  0.0);
// [0, 255] -> [0, 1]
vec3 YCbCr2RGB(vec3 val)
{
	vec3 tmp = val - vec3(16, 128, 128);
	return RGB_CONVERT * tmp;
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
		vec3 recol = recolour_rgb[mask_col - 1];
		vec3 yuv = RGB2YCbCr(col.rgb) * RGB2YCbCr(recol);
		gl_FragColor = vec4(YCbCr2RGB(yuv), col.a);
		return;
	}

	// Gradient shifting
	vec3 new_rgb = clamp(col.rgb + vec3(gradient_shift * STEP_SIZE), vec3(0.0), vec3(1.0));
	gl_FragColor = vec4(new_rgb, col.a);
}
