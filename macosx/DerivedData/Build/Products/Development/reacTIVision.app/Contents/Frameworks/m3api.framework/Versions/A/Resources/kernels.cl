const sampler_t sampleri = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;
const sampler_t samplerf = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_LINEAR;

#define LUT_X 8
#define LUT_Y 8

float lookup_lut(read_only image2d_t lut, const float x) {
	const float row = trunc(clamp(ldexp(x, LUT_Y), 0.0f, (float)(1 << LUT_Y) - 1.0f));
	return read_imagef(lut, samplerf, (float2)(ldexp(x, LUT_Y + LUT_X) - ldexp(row, LUT_X) + 0.5f, row + 0.5f)).x;
}

kernel void colorspaceconv(read_only image2d_t in, write_only image2d_t out, constant float *mat,
		read_only image2d_t inlutr, read_only image2d_t inlutg, read_only image2d_t inlutb,
		read_only image2d_t outlutr, read_only image2d_t outlutg, read_only image2d_t outlutb,
		const char linearin, const char linearout) {
	const int x = get_global_id(0);
	const int y = get_global_id(1);

	float4 pixelin = read_imagef(in, sampleri, (int2)(x, y));
	float4 pixelout;

	if(linearin == 0) {
		pixelin.x = lookup_lut(inlutr, pixelin.x);
		pixelin.y = lookup_lut(inlutg, pixelin.y);
		pixelin.z = lookup_lut(inlutb, pixelin.z);
	}

	pixelout.x = mat[0] * pixelin.x + mat[1] * pixelin.y + mat[2] * pixelin.z;
	pixelout.y = mat[3] * pixelin.x + mat[4] * pixelin.y + mat[5] * pixelin.z;
	pixelout.z = mat[6] * pixelin.x + mat[7] * pixelin.y + mat[8] * pixelin.z;

	if(linearout == 0) {
		pixelout.x = lookup_lut(outlutr, pixelout.x);
		pixelout.y = lookup_lut(outlutg, pixelout.y);
		pixelout.z = lookup_lut(outlutb, pixelout.z);
	}

	write_imagef(out, (int2)(x, y), pixelout);
}
