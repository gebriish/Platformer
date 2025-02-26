#pragma once
#include <cstdint>

struct Window;

enum EventType {
  EVENT_QUIT, 
  EVENT_RESIZE,
  EVENT_KEY,
  EVENT_CURSOR,
  EVENT_MOUSE_BUTTON,
  EVENT_CODEPOINT,
  EVENT_SCROLL,
};

enum WindowFlags {
  WINDOWFLAG_NONE         = 0,
  WINDOWFLAG_MAXIMIZED    = 1 << 0,
  WINDOWFLAG_RESIZABLE    = 1 << 1,
  WINDOWFLAG_VSYNC        = 1 << 2,
  WINDOWFLAG_UNDECORATED  = 1 << 3,
  WINDOWFLAG_CENTERED     = 1 << 4,
  WINDOWFLAG_TRANSPARENT  = 1 << 5,
};

struct Event {
  EventType type;

  union {
    struct { int width, height; } resize_data;
    struct { int key, scancode, action, mods; } key_data;
    struct { double x, y, delta_x, delta_y; } cursor_data;
    struct { int button, action, mods; } mouse_buton_data;
    struct { uint32_t codepoint; } codepoint_data;
    struct { double x, y; } scroll_data;
  };
};

Window* window_initialize(int width, int height, const char* title, uint32_t flags);
void window_destroy(Window* window);

void window_swap_buffers(const Window* window);
void window_poll_events(Window* window);

bool window_get_event(Event& e);
void window_set_title(Window* window, const char* title);

void* window_get_native_handle(const Window* window); 
