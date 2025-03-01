#include "../window.hpp"

#include <queue>
#include <GLFW/glfw3.h>

#define EVENT_QUEUE_SIZE 128

struct Window {
	GLFWwindow* handle;
  bool running;
};

#define EVENT_QUEUE_SIZE 128

static struct {
  std::queue<Event> queue;
} g_EventManager;

Window* window_initialize(int width, int height, const char* title, uint32_t flags) {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, flags & WINDOWFLAG_RESIZABLE   ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_MAXIMIZED, flags & WINDOWFLAG_MAXIMIZED   ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_DECORATED, flags & WINDOWFLAG_UNDECORATED ? GLFW_FALSE : GLFW_TRUE);
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, flags & WINDOWFLAG_TRANSPARENT ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);


#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  Window* result_window = new Window;
  result_window->handle = glfwCreateWindow(width, height, title, NULL, NULL); 
  if(result_window->handle == NULL) {
    delete result_window;

    return nullptr;
  }

  glfwMakeContextCurrent(result_window->handle);
  
  glfwSetKeyCallback(result_window->handle, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    Event key_event;
    key_event.type = EVENT_KEY;
    
    key_event.key_data.key = key;
    key_event.key_data.scancode = scancode;
    key_event.key_data.action = action;
    key_event.key_data.mods = mods;

    g_EventManager.queue.push(key_event);
  });

  glfwSetMouseButtonCallback(result_window->handle, [](GLFWwindow* window, int button, int action, int mods) {
    Event mouse_event;
    mouse_event.type = EVENT_MOUSE_BUTTON;
    
    mouse_event.mouse_buton_data.button = button;
    mouse_event.mouse_buton_data.action = action;
    mouse_event.mouse_buton_data.mods = mods;

    g_EventManager.queue.push(mouse_event);
  });

  glfwSetCursorPosCallback(result_window->handle, [](GLFWwindow* window, double x, double y) {
    Event cursor_event;
    cursor_event.type = EVENT_CURSOR;

    static double last_x = x, last_y = y;

    cursor_event.cursor_data.x = x;
    cursor_event.cursor_data.y = y;
    cursor_event.cursor_data.delta_x = x - last_x;
    cursor_event.cursor_data.delta_y = y - last_y;

    last_x = x;
    last_y = y;

    g_EventManager.queue.push(cursor_event);
  });

  glfwSetScrollCallback(result_window->handle, [](GLFWwindow* window, double x, double y) {
    Event scroll_event;
    scroll_event.type = EVENT_SCROLL;

    scroll_event.scroll_data.x = x;
    scroll_event.scroll_data.y = y;

    g_EventManager.queue.push(scroll_event);
  });

  glfwSetFramebufferSizeCallback(result_window->handle, [](GLFWwindow* window, int width, int height) {
    Event resize_event;
    resize_event.type = EVENT_RESIZE;

    resize_event.resize_data.width = width;
    resize_event.resize_data.height = height;

    g_EventManager.queue.push(resize_event);
  });

  glfwSetCharCallback(result_window->handle, [](GLFWwindow* window, uint32_t codepoint) {
    Event codepoint_event;
    codepoint_event.type = EVENT_CODEPOINT;

    codepoint_event.codepoint_data.codepoint = codepoint;

    g_EventManager.queue.push(codepoint_event);
  });

  if(flags & WINDOWFLAG_CENTERED)
  {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwSetWindowPos(result_window->handle, 
        (mode->width - width)/2,
        (mode->height - height)/2);
  }

  glfwSwapInterval(flags & WINDOWFLAG_VSYNC ? 1 : 0);

  glfwShowWindow(result_window->handle);

  result_window->running = true;
  return result_window;
}

void window_destroy(Window* window) {
  glfwDestroyWindow(window->handle);
  delete window;
}

void window_swap_buffers(const Window* window) {

  glfwSwapBuffers(window->handle);
}

void window_poll_events(Window* window) {
  glfwPollEvents();

  if(glfwWindowShouldClose(window->handle) && window->running) {
    Event quit_event;
    quit_event.type = EVENT_QUIT;
    window->running = false;
    g_EventManager.queue.push(quit_event);
  }
}

bool window_get_event(Event& e) {
  if(g_EventManager.queue.size() == 0) return false;

  e = g_EventManager.queue.front();
  g_EventManager.queue.pop();

  return true;
}

void window_set_title(Window* window, const char* title) {
	glfwSetWindowTitle(window->handle, title);
}

void* window_get_native_handle(const Window* window) {
  return (void*)window->handle;
}

std::pair<int,int> window_get_resolution(const Window* window) {
	if (!window) 
		return std::make_pair(0,0);

	int width, height;
	glfwGetWindowSize(window->handle, &width, &height);
	return std::make_pair(width, height);
}
