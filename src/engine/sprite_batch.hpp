#pragma once
#include <cstdlib>
#include <array>
#include "vmath.hpp"

struct Sprite {
	vec2 position;
	vec2 size;
	vec2 uvmin, uvmax;
	vec4 color;
	float texid;
};

class SpriteBatch {
public:
	SpriteBatch();
	~SpriteBatch();

	void begin();
	void end();
	bool add(const Sprite& sprite);
	void drawBatch();

private:
	unsigned int vao, quad_vbo, instance_vbo, ebo;
	static constexpr size_t MAX_SPRITES = 1024;

	std::array<Sprite, MAX_SPRITES> instance_data;
	size_t sprite_count = 0;

	void _init();
};
