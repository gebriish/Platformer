#include "../action_map.hpp"
#include "../window.hpp"

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <vector>
#include <cstdio>

static struct {
  std::unordered_map<std::string, std::vector<ActionBinding>> bindings;
  std::unordered_map<std::string, Action> action_states;
} g_ActionMap;

static float get_input_value(GLFWwindow* window, const ActionBinding& bind) {
  switch(bind.type) {
    case ActionType::KEYBOARD :
      return glfwGetKey(window, bind.key) == GLFW_PRESS ? 1.0f : 0.0f;
    case ActionType::MOUSE_BUTTON :
      return glfwGetMouseButton(window, bind.key) == GLFW_PRESS ? 1.0f : 0.0f;
    case ActionType::MOUSE_AXIS : {
      double xpos, ypos;
      glfwGetCursorPos(window, &xpos, &ypos);
      return (bind.key == 0) ? float(xpos) : float(ypos);
    }
    case ActionType::GAMEPAD_BUTTON : {
      GLFWgamepadstate state;
      if(glfwGetGamepadState(GLFW_JOYSTICK_1, &state)) {
        return state.buttons[bind.key] == GLFW_PRESS ? 1.0f : 0.0f;
      }
      return 0.0f;
    }
    case ActionType::GAMEPAD_AXIS : {
      GLFWgamepadstate state;
      if(glfwGetGamepadState(GLFW_JOYSTICK_1, &state)) {
        return state.axes[bind.key];
      }
      return 0.0f;
    }
  }

  return 0.0f;
}

void action_map_update(Window* window) {
  for (auto& [action, state] : g_ActionMap.action_states) {
    state.prev_state = state.current_state;
  }

  for (auto& [action, binds] : g_ActionMap.bindings) {
    Action& state = g_ActionMap.action_states[action];

    bool pressed = false;
    float value = 0.0f;
    int value_sum_count = 0;

    for (const auto& bind : binds) {
      float value_reading = get_input_value((GLFWwindow*)window_get_native_handle(window), bind);
      if (std::abs(value_reading) > bind.dead_zone) {
        pressed = true;
        value = value * value_sum_count + value_reading * bind.scale;
        value /= ++value_sum_count;
      }
    }

    state.current_state = pressed;
    state.value = value;
  }
}

void action_bind_key(const std::string& name, int key, float scale) {
  ActionBinding bind = {
    .type = ActionType::KEYBOARD,
    .key = key,
    .scale = scale
  };

  g_ActionMap.bindings[name].push_back(bind);
}

void action_bind_mouse_button(const std::string& name, int button) {
  ActionBinding bind = {
    .type = ActionType::MOUSE_BUTTON,
    .key = button
  };

  g_ActionMap.bindings[name].push_back(bind);
}


void action_bind_mouse_axis(const std::string& name, bool x_axis, 
                            float dead_zone, float scale) {

  ActionBinding bind = {
    .type = ActionType::MOUSE_AXIS,
    .key = x_axis ? 0 : 1,
    .scale = scale,
    .dead_zone = dead_zone
  };

  g_ActionMap.bindings[name].push_back(bind);
}


void action_bind_gamepad_button(const std::string& name, int button) {
  ActionBinding bind = {
    .type = ActionType::GAMEPAD_BUTTON,
    .key = button
  };

  g_ActionMap.bindings[name].push_back(bind);
}

void action_bind_gamepad_axis(const std::string& name, bool x_axis, 
                              float dead_zone, float scale) {

  ActionBinding bind = {
    .type = ActionType::GAMEPAD_AXIS,
    .key = x_axis ? 0 : 1,
    .scale = scale,
    .dead_zone = dead_zone
  };

  g_ActionMap.bindings[name].push_back(bind);
}

bool action_pressed(const std::string& name) {
  auto it = g_ActionMap.action_states.find(name);
  if(it != g_ActionMap.action_states.end()) {
    const Action& action = it->second;
    return action.current_state;
  }

  return false;

}

bool action_just_pressed(const std::string& name) {
  auto it = g_ActionMap.action_states.find(name);
  if(it != g_ActionMap.action_states.end()) {
    const Action& action = it->second;
    return action.current_state && !action.prev_state;
  }

  return false;
}

bool action_just_released(const std::string& name) {
  auto it = g_ActionMap.action_states.find(name);
  if(it != g_ActionMap.action_states.end()) {
    const Action& action = it->second;
    return !action.current_state && action.prev_state;
  }

  return false;
}

float action_get_value(const std::string& name) {
  auto it = g_ActionMap.action_states.find(name);
  return (it != g_ActionMap.action_states.end()) ? it->second.value : 0.0f;
}
