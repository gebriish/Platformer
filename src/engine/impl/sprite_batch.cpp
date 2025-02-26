#include "../sprite_batch.hpp"
#include <iostream>
#include <glad/glad.h>

SpriteBatch::SpriteBatch() 
{
	_init();
}

SpriteBatch::~SpriteBatch()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &quad_vbo);
	glDeleteBuffers(1, &instance_vbo);
	glDeleteBuffers(1, &ebo);
}

void SpriteBatch::begin()
{
	sprite_count = 0;
}

bool SpriteBatch::add(const Sprite& sprite) 
{
	if(sprite_count >= MAX_SPRITES) return false;
	instance_data[sprite_count++] = sprite;
	return true;
}

void SpriteBatch::end() 
{
	glBindBuffer(GL_ARRAY_BUFFER, instance_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sprite_count * sizeof(Sprite), instance_data.data());
}

void SpriteBatch::drawBatch() 
{
	if (sprite_count == 0) return;

	glBindVertexArray(vao);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, sprite_count);
	glBindVertexArray(0);
}

void SpriteBatch::_init() 
{
	float quad_vertices[] = {
		0, 0,
		1, 0,
		1, 1,
		0, 1,
	};

	unsigned int quad_indices[] = {
		0, 1, 2, 2, 3, 0,
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), quad_indices, GL_STATIC_DRAW);

	glGenBuffers(1, &instance_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, instance_vbo);
	glBufferData(GL_ARRAY_BUFFER, MAX_SPRITES * sizeof(Sprite), nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)offsetof(Sprite, position));
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)offsetof(Sprite, size));
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)offsetof(Sprite, uvmin));
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);

	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)offsetof(Sprite, uvmax));
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, 1);

	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)offsetof(Sprite, color));
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(5, 1);

	glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)offsetof(Sprite, texid));
	glEnableVertexAttribArray(6);
	glVertexAttribDivisor(6, 1);
	
	glBindVertexArray(0);
}
