#pragma once
#include "vmath.hpp"
#include "color.hpp"
#include "transform.hpp"
#include "window.hpp"
#include "image.hpp"

struct Camera
{
	Transform transform;
	mat4 proj, view;
	bool orthographic = false;

	union 
	{
		struct { float fov, nearClip, farClip, aspectRatio; } perspectiveProperties;
		struct { float nearClip, farClip, left, right, top, bottom; } orthographicProperties;
	};
	
	void updateProjection();
	void updateView();
};

struct Texture2D {
	unsigned int id;
};

enum TextureSpec : uint8_t {
	TEXTURESPEC_NONE   = 0,
	TEXTURESPEC_LINEAR = 1 << 0,
	TEXTURESPEC_CLIP   = 1 << 1,
};

void renderer_init(const Window* window);
void renderer_cleanup();
void renderer_set_viewport(int x, int y);

void renderer_draw_lines(const Camera& render_cam);
void renderer_draw_sprites(const Camera& render_cam);

void renderer_clear_buffer(float r, float g, float b, float a);
void renderer_clear_buffer(float color[4]);

void renderer_add_line2d(float x1, float y1, float x2, float y2, 
												 const Color& color = Color { 1.0f, 1.0f, 1.0f, 1.0f});
void renderer_add_box2d(float x, float y, float w, float h, const Color& color = {1,1,1,1});
void renderer_add_sprite(float x, float y, float w, float h, const Color& color = {1,1,1,1},
	 int tex = 0, float uvx1 = 0.0f, float uvy1 = 0.0f, float uvx2 = 1.0f, float uvy2 = 1.0f);

void renderer_bind_texture_slot(Texture2D texture, int slot);

void renderer_clear_sprite_buffer();
void renderer_clear_line_buffer();

Texture2D renderer_load_texture(Image2D* image, TextureSpec spec);
Texture2D renderer_load_texture(const char* path, TextureSpec spec);
void renderer_delete_texture(Texture2D& texture);

void renderer_begin();
void renderer_end();
void renderer_blit(const Window* window);
