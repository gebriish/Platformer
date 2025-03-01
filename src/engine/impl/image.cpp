#include "../image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

struct Image2D {
	int width, height, channels;
	unsigned char* data; 
	bool stbi_allocated;
};

Image2D* image_load(const char* path) {
	Image2D* img = new Image2D;
	img->data = stbi_load(path, &img->width, &img->height, &img->channels, 0);
	img->stbi_allocated = true;

	if (!img->data) {
		delete img;
		return nullptr;
	}

	return img;
}

Image2D*  image_initialize(int width, int height, int channels) {
	Image2D* image = new Image2D;
	image->width = width;
	image->height = height;
	image->channels = channels;
	image->data = new unsigned char[width * height * channels];
	image->stbi_allocated = false;
	return image;
}

void image_free(Image2D* image) {
	if (!image) return;

	if (image->data) {
		if (image->stbi_allocated) {
			stbi_image_free(image->data);
		} else {
			delete[] image->data;
		}
		image->data = nullptr;
	}

	delete image;
}

bool image_get_pixel_handle(const Image2D* image, int x, int y, size_t& handle) {
	if(!image) return false;

	if (x >= image->width || y >= image->height) return false;

	handle = (y * image->width + x) * image->channels;
	return true;
}

bool image_get_specification(const Image2D* image, ImageSpec spec, float& data) {
	if(!image) return false;

	switch(spec) {
		case ImageSpec::WIDTH :
			data = image->width;
			break;
		case ImageSpec::HEIGHT :
			data = image->height;
			break;
		case ImageSpec::NUM_CHANNELS :
			data = image->channels;
			break;
		default:
			return false;
	}

	return true;
}

bool image_write_pixel(const Image2D* image, size_t handle, unsigned char value) {
	if(!image || !image->data) return false;
	image->data[handle] = value;
	return true;
}


const unsigned char* image_get_data(const Image2D* image) {
	if (!image) return nullptr;
	return image->data;
}
