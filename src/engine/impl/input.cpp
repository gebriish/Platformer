#include "../input.hpp"
#include <GLFW/glfw3.h>

//======================================================================//
//                INPUT POLLING UTILITY FUNCTIONS                       //
//======================================================================//

static struct {
  void* window_handle = nullptr;
} g_InputState;

void input_set_window(const Window* window) {
  g_InputState.window_handle = window_get_native_handle(window);
}

bool input_is_key_pressed(int key) {
  GLFWwindow* ptr = (GLFWwindow*) g_InputState.window_handle;
  
  if(ptr)
    return glfwGetKey(ptr, key) == GLFW_PRESS;

  return false;
}

bool input_is_mouse_pressed(int button) {
  GLFWwindow* ptr = (GLFWwindow*) g_InputState.window_handle;
  if(ptr)  
    return glfwGetMouseButton(ptr, button) == GLFW_PRESS;

  return false;
}

std::pair<float, float> input_get_cursor_position() {
  double x, y;
  GLFWwindow* ptr = (GLFWwindow*) g_InputState.window_handle;

	if(ptr)
	{
		glfwGetCursorPos(ptr, &x, &y);
		return std::make_pair(float(x), float(y));
	}

  return std::make_pair(0.0f, 0.0f);
}


bool input_gamepad_conntected(int gamepad) {
  return glfwJoystickPresent(gamepad);
}

bool input_gamepad_button_pressed(int gamepad, int button) {
  if (!glfwJoystickPresent(gamepad)) return false;

  GLFWgamepadstate state;
  if (glfwGetGamepadState(gamepad, &state)) {
    return state.buttons[button] == GLFW_PRESS;
  }
  return false;
}

float input_gamepad_axis(int gamepad, int axis) {
  if (!glfwJoystickPresent(gamepad)) return 0.0f;

  GLFWgamepadstate state;
  if (glfwGetGamepadState(gamepad, &state)) {
    return state.axes[axis];
  }
  return 0.0f;
}
