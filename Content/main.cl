__kernel void main(__write_only image2d_t image)
{
	const int2 uv = { get_global_id(0), get_global_id(1) };
	const int2 dim = get_image_dim(image);
	const int2 grid = { 16, 16 };
	const int2 repeate = dim / grid;
	const uint4 color = { ((uint)uv.x % grid.x) * repeate.x, ((uint)uv.y % grid.y) * repeate.y, 0, 255 };
	write_imageui(image, uv, color);
}