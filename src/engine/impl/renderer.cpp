#include "../renderer.hpp"
#include "../command_buffer.hpp"
#include "../vmath.hpp"
#include "../shader_program.hpp"
#include "../line_batch.hpp"
#include "../sprite_batch.hpp"

#include "../shaders/line_shader.hpp"
#include "../shaders/sprite_shader.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

enum class DrawCommandType {
	NONE,

	LINE2D,
	BOX2D,

	SPRITE,
};

enum RendererNumericLimits {
	RNL_IMAGE_BIND_LIMIT = 32,
};

struct DrawCommand {
	DrawCommand(DrawCommandType type = DrawCommandType::NONE)
	: type(type) 
	{}

	DrawCommandType type;
	union {
		struct { vec2 p1, p2; Color color; } line_2d_data;
		struct { vec2 point, size; Color color; } box_2d_data;
		struct { vec2 pos, size, uv_min, uv_max; int tex; Color color; } sprite_data;
	};
};

static struct {
	bool initialized = false; 

	//========================================================
	DynamicCommandBuffer<DrawCommand, 1024> line_command_buffer;
	DynamicCommandBuffer<DrawCommand, 1024> sprite_command_buffer;

	//========================================================
	LineBatch2D* line_batch_2d;
	SpriteBatch* sprite_batch;

	//========================================================
	Shader line_shader;
	Shader sprite_shader;

	//========================================================
	int texture_slots[RNL_IMAGE_BIND_LIMIT];
	int bind_slot_max;
	Texture2D white_texture;
} g_RendererState;

void Camera::updateProjection()
{
	if(!orthographic)
		proj = mat4::perspective(perspectiveProperties.fov,
													 perspectiveProperties.aspectRatio, 
													 perspectiveProperties.nearClip, 
													 perspectiveProperties.farClip);
	else
	{
		proj = mat4::orthographic(
			orthographicProperties.left,
			orthographicProperties.right,
			orthographicProperties.bottom,
			orthographicProperties.top,
			orthographicProperties.nearClip, 
			orthographicProperties.farClip 
		);
	}
}

void Camera::updateView()
{
	view = mat4::lookAt(transform.getGlobalPosition(), transform.getGlobalPosition() + transform.forward(), transform.up());
}


static void add_line_2d(const DrawCommand& line) 
{
	Line2D l {
		.begin = line.line_2d_data.p1,
		.end = line.line_2d_data.p2,
		.color = {line.line_2d_data.color.r,
			line.line_2d_data.color.g, 
			line.line_2d_data.color.b,
			line.line_2d_data.color.a
		}
	};

	if(!g_RendererState.line_batch_2d->addLine(l)) {
		g_RendererState.line_batch_2d->end();
		g_RendererState.line_batch_2d->drawBatch();
		g_RendererState.line_batch_2d->begin();

		g_RendererState.line_batch_2d->addLine(l);
	}
}

static void add_sprite(const DrawCommand& sprite)
{
	Sprite s {
		.position = sprite.sprite_data.pos,
		.size = sprite.sprite_data.size,
		.uvmin = sprite.sprite_data.uv_min,
		.uvmax = sprite.sprite_data.uv_max,
		.color = vec4(sprite.sprite_data.color.r, sprite.sprite_data.color.g, sprite.sprite_data.color.b, sprite.sprite_data.color.a),
		.texid = (float) sprite.sprite_data.tex
	};

	if(!g_RendererState.sprite_batch->add(s)) {
		g_RendererState.sprite_batch->end();
		g_RendererState.sprite_batch->drawBatch();
		g_RendererState.sprite_batch->begin();

		g_RendererState.sprite_batch->add(s);
	}
}

static void add_box_2d(const DrawCommand& box) 
{
	DrawCommand l(DrawCommandType::LINE2D);
	l.line_2d_data = {
		.p1 = vec2(box.box_2d_data.point.x, box.box_2d_data.point.y),
		.p2 = vec2(box.box_2d_data.point.x + box.box_2d_data.size.x, box.box_2d_data.point.y),
		.color = box.box_2d_data.color
	};
	add_line_2d(l);
	l.line_2d_data = {
		.p1 = vec2(box.box_2d_data.point.x + box.box_2d_data.size.x, box.box_2d_data.point.y),
		.p2 = vec2(box.box_2d_data.point.x + box.box_2d_data.size.x, box.box_2d_data.point.y + box.box_2d_data.size.y),
		.color = box.box_2d_data.color
	};
	add_line_2d(l);
	l.line_2d_data = {
		.p1 = vec2(box.box_2d_data.point.x + box.box_2d_data.size.x, box.box_2d_data.point.y + box.box_2d_data.size.y),
		.p2 = vec2(box.box_2d_data.point.x, box.box_2d_data.point.y + box.box_2d_data.size.y),
		.color = box.box_2d_data.color
	};
	add_line_2d(l);
	l.line_2d_data = {
		.p1 = vec2(box.box_2d_data.point.x, box.box_2d_data.point.y + box.box_2d_data.size.y),
		.p2 = vec2(box.box_2d_data.point.x, box.box_2d_data.point.y),
		.color = box.box_2d_data.color
	};
	add_line_2d(l);
}

void renderer_init(const Window* window) {
	if(g_RendererState.initialized) {
		std::cout << "Renderer already initialized\n";
		return;
	}

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD\n";
		return;
	}

	std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	g_RendererState.line_batch_2d = new LineBatch2D(1024);
	g_RendererState.sprite_batch  = new SpriteBatch();

	shader_load_glsl_from_source(LINE2D_SHADER_SOURCE_V, 
															LINE2D_SHADER_SOURCE_F, 
															g_RendererState.line_shader);
	shader_load_glsl_from_source(SPRITE_SHADER_SOURCE_V,
															SPRITE_SHADER_SOURCE_F,
															g_RendererState.sprite_shader);

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &g_RendererState.bind_slot_max);
	g_RendererState.bind_slot_max = MIN(g_RendererState.bind_slot_max, RNL_IMAGE_BIND_LIMIT);

	int texture_slots[GL_MAX_TEXTURE_IMAGE_UNITS];
	for (size_t i=0; i<g_RendererState.bind_slot_max; i++) {
		texture_slots[i] = i;
	}

	shader_use_program(g_RendererState.sprite_shader);
	shader_upload_int_array(g_RendererState.sprite_shader, "uTextures", 
												 texture_slots, g_RendererState.bind_slot_max);
	shader_use_program({0});
 
	{
		Image2D* white = image_initialize(1,1,3);
		size_t handle;
		image_get_pixel_handle(white, 0,0,handle);
		image_write_pixel(white, handle  , 255);
		image_write_pixel(white, handle+1, 255);
		image_write_pixel(white, handle+2, 255);

		g_RendererState.white_texture = renderer_load_texture(white, TEXTURESPEC_NONE);
		image_free(white);
	}
}

void renderer_set_viewport(int x, int y) 
{
	glViewport(0, 0, x, y);
}

void renderer_cleanup() 
{
	renderer_delete_texture(g_RendererState.white_texture);
	
	g_RendererState.line_command_buffer.clear();
	g_RendererState.sprite_command_buffer.clear();

	delete g_RendererState.line_batch_2d;
	delete g_RendererState.sprite_batch;

	shader_delete_program(g_RendererState.line_shader);
	shader_delete_program(g_RendererState.sprite_shader);

	g_RendererState.initialized = false;
}

void renderer_draw_lines(const Camera& camera) 
{
	shader_use_program(g_RendererState.line_shader);
	shader_upload_mat4(g_RendererState.line_shader, "uProj", camera.proj.elements);
	shader_upload_mat4(g_RendererState.line_shader, "uView", camera.view.elements);

	g_RendererState.line_batch_2d->begin();

	DrawCommand line_command;
	while(g_RendererState.line_command_buffer.pop_front(line_command)) {
		switch (line_command.type)
		{
			case DrawCommandType::LINE2D:
				add_line_2d(line_command);
				break;
			case DrawCommandType::BOX2D:
				add_box_2d(line_command);
				break;
			default:
				break;
		}
	}

	g_RendererState.line_batch_2d->end();
	g_RendererState.line_batch_2d->drawBatch();
}

void renderer_draw_sprites(const Camera& camera) 
{

	shader_use_program(g_RendererState.sprite_shader);
	shader_upload_mat4(g_RendererState.sprite_shader, "uProj", camera.proj.elements);
	shader_upload_mat4(g_RendererState.sprite_shader, "uView", camera.view.elements);

	renderer_bind_texture_slot(g_RendererState.white_texture, 0);

	for(size_t i=0; i<g_RendererState.bind_slot_max; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, g_RendererState.texture_slots[i]);
	}

	g_RendererState.sprite_batch->begin();

	DrawCommand sprite_command;
	while(g_RendererState.sprite_command_buffer.pop_front(sprite_command)) {
		add_sprite(sprite_command);
	}

	g_RendererState.sprite_batch->end();
	g_RendererState.sprite_batch->drawBatch();

}

void renderer_clear_buffer(float r, float g, float b, float a) 
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_clear_buffer(float color[4]) 
{
	glClearColor(color[0], color[1], color[2], color[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_add_line2d(float x1, float y1, 
												 float x2, float y2, 
												 const Color& color) 
{
	DrawCommand command (DrawCommandType::LINE2D);

	command.line_2d_data.p1    = vec2(x1, y1);
	command.line_2d_data.p2    = vec2(x2, y2);
	command.line_2d_data.color = color;

	g_RendererState.line_command_buffer.push_command(command);
}

void renderer_add_box2d(float x, float y, float w, float h, const Color& color)
{
	DrawCommand command (DrawCommandType::BOX2D);

	command.box_2d_data.point = vec2(x, y);
	command.box_2d_data.size = vec2(w, h);
	command.box_2d_data.color = color;

	g_RendererState.line_command_buffer.push_command(command);
}

void renderer_add_sprite(float x, float y, float w, float h, const Color& color,
												 int tex, float uvx1, float uvy1, float uvx2, float uvy2)
{
	DrawCommand command(DrawCommandType::SPRITE);

	command.sprite_data = {
		vec2(x, y),
		vec2(w, h),
		vec2(uvx1, uvy1),
		vec2(uvx2, uvy2),
		tex,
		color
	};

	g_RendererState.sprite_command_buffer.push_command(command);
}

void renderer_bind_texture_slot(Texture2D texture, int slot) {
	if (slot <  g_RendererState.bind_slot_max) {
		g_RendererState.texture_slots[slot] = texture.id;
	}
}

void renderer_clear_line_buffer() 
{
	g_RendererState.line_command_buffer.clear();
}

void renderer_clear_sprite_buffer() {
	g_RendererState.sprite_command_buffer.clear();
}

Texture2D renderer_load_texture(Image2D* image, TextureSpec spec) {
	if(!image || !image_get_data(image))
		return Texture2D {0};

	Texture2D texture;
	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);

	GLint filter = (spec & TEXTURESPEC_LINEAR) ? GL_LINEAR : GL_NEAREST;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	GLint wrap = (spec & TEXTURESPEC_CLIP) ? GL_CLAMP_TO_EDGE : GL_REPEAT;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	
	int channels, width, height;

	{
		float data;
		image_get_specification(image, ImageSpec::NUM_CHANNELS, data);
		channels = int(data);
		image_get_specification(image, ImageSpec::WIDTH, data);
		width = int(data);
		image_get_specification(image, ImageSpec::HEIGHT, data);
		height = int(data);
	}

	GLenum format = (channels == 4) ? GL_RGBA :
		(channels == 3) ? GL_RGB  :
		(channels == 2) ? GL_RG   :
		GL_RED;

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, 
							format, GL_UNSIGNED_BYTE, image_get_data(image));
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

Texture2D renderer_load_texture(const char* path, TextureSpec spec) {
	Texture2D texture;
	Image2D* image = image_load(path);
	texture = renderer_load_texture(image, spec);
	image_free(image);
	return texture;
}

void renderer_delete_texture(Texture2D& texture) {
	glDeleteTextures(1, &texture.id);
	texture.id = 0;
}

void renderer_begin() 
{
	//TODO: implement a render target
}

void renderer_end()
{
	//TODO: implement a render target
}

void renderer_blit(const Window* window) 
{
	//TODO: implement a render target
}
