#include "engine/core.hpp"
#include "engine/window.hpp"
#include "engine/renderer.hpp"
#include "engine/input.hpp"
#include "engine/action_map.hpp"
#include "engine/image.hpp"

#include <cstdio>
#include <iostream>

static Window* window = nullptr;
static Camera camera;

int main() {
	core_init();

	window = window_initialize(
		800,
		500,
		"ʕっ•ᴥ•ʔっ", 
		WINDOWFLAG_CENTERED | WINDOWFLAG_RESIZABLE
	);

	if(!window) {
		core_destroy();
		return -1;
	}
	input_set_window(window);

	renderer_init();

	bool running = true;

	camera.orthographic = true;
	camera.orthographicProperties.nearClip = -1;
	camera.orthographicProperties.farClip = 1000;

	camera.orthographicProperties.left = -400;
	camera.orthographicProperties.top = -250;
	camera.orthographicProperties.right = 400;
	camera.orthographicProperties.bottom = 250;

	action_bind_key("Horizontal", KEY_D, 1.0f);
	action_bind_key("Horizontal", KEY_A,-1.0f);
	action_bind_key("Vertical", KEY_W, 1.0f);
	action_bind_key("Vertical", KEY_S,-1.0f);

	vec2 panel_pos(0.0f);
	vec2 panel_size(128.0f);

	const Color clear_color = color_from_hexcode("232323");

	Texture2D t = renderer_load_texture("res/image.png", TEXTURESPEC_NONE);
	renderer_bind_texture_slot(t, 1);

	while(running) {
		core_update();
		float deltatime = core_deltatime();

		Event e;
		while(window_get_event(e)) {
			if(e.type == EVENT_QUIT) {
				running = false;
				break;
			}
			else if (e.type == EVENT_CURSOR) {
				if(input_is_mouse_pressed(0))
					panel_pos += vec2(e.cursor_data.delta_x, e.cursor_data.delta_y);
				else if(input_is_mouse_pressed(1)) {
					panel_size += vec2(e.cursor_data.delta_x, e.cursor_data.delta_y);
					panel_size = MAX(vec2(16), panel_size);
				}
			}
			else if (e.type == EVENT_RESIZE) {
				int width = e.resize_data.width / 2;
				int height = e.resize_data.height / 2;
				camera.orthographicProperties.left = -width;
				camera.orthographicProperties.top = -height;
				camera.orthographicProperties.right = width;
				camera.orthographicProperties.bottom = height;
				renderer_set_viewport(e.resize_data.width, e.resize_data.height);
			}
		}

		vec2 move_vec;
		move_vec.x =  action_get_value("Horizontal");
		move_vec.y = -action_get_value("Vertical");
		move_vec = normalize(move_vec);
		camera.transform.position += vec3(move_vec.x, move_vec.y, 0) * 600 * deltatime;



		auto [x, y] = input_get_cursor_position();
		
		renderer_add_sprite(panel_pos.x, panel_pos.y, 208*3, 288*3, 
											color_from_hexcode("ffffff"),1);


		camera.updateProjection();
		camera.updateView();

		renderer_clear_buffer(clear_color.r, clear_color.g, clear_color.b, clear_color.a);

		renderer_draw_sprites(camera);
		renderer_draw_lines(camera);

		action_map_update(window);
		window_swap_buffers(window);
		window_poll_events(window);
	}

	renderer_delete_texture(t);

	renderer_cleanup();
	core_destroy();
}
