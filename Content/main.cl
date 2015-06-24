__kernel void main(__write_only image2d_t image)
{
	const int2 uv = { get_global_id(0), get_global_id(1) };
	const uint4 color = { ((uint)uv.x % 32) * 8, ((uint)uv.y % 32) * 8, 0, 255 };
	write_imageui(image, uv, color);
}