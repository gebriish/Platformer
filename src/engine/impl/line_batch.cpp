#include "../line_batch.hpp"
#include <glad/glad.h>


//======================================================================//
//                       2D BATCHED LINE MESH                           //
//======================================================================//

LineBatch2D::LineBatch2D(size_t capacity)
: BATCH_SIZE(capacity), vertex_array(capacity * 2) 
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, BATCH_SIZE * 2 * sizeof(LineVertex2D), nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(LineVertex2D), (void*)offsetof(LineVertex2D, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(LineVertex2D), (void*)offsetof(LineVertex2D, color));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

LineBatch2D::~LineBatch2D() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void LineBatch2D::begin() {
	batch_open = true;
	stack_top = 0;
}

void LineBatch2D::end() {
	batch_open = false;
}

bool LineBatch2D::addLine(const Line2D& line) {
	if (stack_top >= BATCH_SIZE || !batch_open)
		return false;

	size_t index = stack_top * 2;
	vertex_array[index] = { line.begin, line.color };
	vertex_array[index + 1] = { line.end, line.color };

	stack_top++;
	return true;
}

void LineBatch2D::drawBatch() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferSubData(GL_ARRAY_BUFFER, 0, stack_top * 2 * sizeof(LineVertex2D), vertex_array.data());

	glDrawArrays(GL_LINES, 0, 2 * stack_top);
  glBindVertexArray(0);
}
