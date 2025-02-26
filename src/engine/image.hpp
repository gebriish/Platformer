#pragma once
#include <cstdlib>

enum class ImageSpec {
	WIDTH,
	HEIGHT,
	NUM_CHANNELS,
};

struct Image2D;

Image2D* image_load(const char* path);
Image2D* image_initialize(int width, int height, int channels);
void image_free(Image2D* image);

bool image_get_pixel_handle(Image2D* image, int x, int y, size_t& handle);
bool image_get_specification(Image2D* image, ImageSpec spec, float& data);
bool image_write_pixel(Image2D* image, size_t handle, unsigned char value);
unsigned char* image_get_data(Image2D* image);



