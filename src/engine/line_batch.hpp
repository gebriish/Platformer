#pragma once
#include <vector>
#include "vmath.hpp"

struct Line2D {
	vec2 begin;
	vec2 end;
	vec4 color;
};

struct LineVertex2D {
	vec2 position;
	vec4 color;
};

class LineBatch2D {
public:
	explicit LineBatch2D(size_t capacity);
	~LineBatch2D();

	void begin();
	void end();
	bool addLine(const Line2D& line);
	void drawBatch();

private:
	std::vector<LineVertex2D> vertex_array;
	unsigned int vao, vbo;
	size_t stack_top = 0;
	bool batch_open = false;
	const size_t BATCH_SIZE;
};
