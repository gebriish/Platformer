#include "engine/core.hpp"
#include "engine/window.hpp"
#include "engine/renderer.hpp"
#include "engine/input.hpp"
#include "engine/action_map.hpp"

#include <cstdio>
#include <iostream>

static Window* window = nullptr;
static Camera camera;

int main() {
	bool running = true;

	//===========================================================
	// Application Initialization
	//===========================================================
	{
		core_init();

		window = window_initialize(
			640,
			480,
			"ʕっ•ᴥ•ʔっ", 
			WINDOWFLAG_CENTERED | WINDOWFLAG_RESIZABLE
		);

		if(!window) {
			core_destroy();
			return -1;
		}

		input_set_window(window);
		renderer_init(window);
	}
	
	//===========================================================
	// Camera Setup
	//===========================================================
	{
		auto[w, h] = window_get_resolution(window);
		camera.transform.position = vec3(0,0,0);
		camera.orthographic = true;
		camera.orthographicProperties = {
			-1,
			1,
			-float(w)/2,
			float(w)/2,
			-float(h)/2,
			float(h)/2,
		};
	}

	vec2 panel_pos(0.0f), panel_size(32);

	const Color clear_color = color_from_hexcode("312e2f");

	while(running) {
		core_update();
		float deltatime = core_deltatime();

		//===========================================================
		// Window event handling
		//===========================================================
		Event e;
		while(window_get_event(e))  {
			if(e.type == EVENT_QUIT) {
				running = false;
				break;
			}
			else if (e.type == EVENT_CURSOR) {
				if(input_is_mouse_pressed(0))
					panel_pos += vec2(e.cursor_data.delta_x, e.cursor_data.delta_y);
				else if ( input_is_mouse_pressed(1) ) 
					panel_size += vec2(e.cursor_data.delta_x, e.cursor_data.delta_y);
			}
			else if (e.type == EVENT_RESIZE) {
				auto[w, h] = window_get_resolution(window);
				camera.orthographicProperties = {
					-1,
					1,
					-float(w)/2,
					float(w)/2,
					-float(h)/2,
					float(h)/2,
				};
				renderer_set_viewport(w, h);
			}
		}

		auto [x, y] = input_get_cursor_position();
		renderer_add_sprite(panel_pos.x, panel_pos.y, panel_size.x,panel_size.y, 
											color_from_hexcode("ffedd4"));
	

		//===========================================================
		// Game state Render
		//===========================================================

		camera.updateProjection();
		camera.updateView();

		renderer_begin();
		renderer_clear_buffer(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
		renderer_draw_lines(camera);
		renderer_draw_sprites(camera);
		renderer_end();

		renderer_blit(window);

		//===========================================================
		// Prep for next frame
		//===========================================================
		action_map_update(window);
		window_swap_buffers(window);
		window_poll_events(window);
	}

	//===========================================================
	// Application cleanup
	//===========================================================
	{
		renderer_cleanup();
		core_destroy();
	}
}
