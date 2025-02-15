#include "engine/core.hpp"
#include "engine/window.hpp"
#include "engine/action_map.hpp"
#include "engine/input.hpp"

static Window* window = nullptr;

int main() {
  core_init();

  window = window_initialize(
    800, 
    500, 
    "Platformer", 
    WINDOWFLAG_CENTERED | WINDOWFLAG_RESIZABLE | WINDOWFLAG_VSYNC
  );

  if(!window) {
    core_destroy();
    return -1;
  }

  bool running = true;

  action_bind_key("HOR", KEY_W, 1.0f);
  action_bind_key("HOR", KEY_S, -1.0f);

  action_bind_key("VERT", KEY_D, 1.0f);
  action_bind_key("VERT", KEY_A, -1.0f);

  while(running) {
    Event e;
    while(window_get_event(e)) {
      if(e.type == EVENT_QUIT) {
        running = false;
        break;
      }
    }


    action_map_update(window);
    window_swap_buffers(window);
    window_poll_events(window);
  }

  core_destroy();
}
