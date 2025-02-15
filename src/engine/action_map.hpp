#pragma once
#include <string>
#include "window.hpp"

struct Action {
  bool current_state = false;
  bool prev_state = false;
  float value = 0.0f;
};

enum class ActionType {
  KEYBOARD,
  MOUSE_BUTTON,
  MOUSE_AXIS,
  GAMEPAD_BUTTON,
  GAMEPAD_AXIS
};

struct ActionBinding {
  ActionType type;
  int key;
  float scale = 1.0f;
  float dead_zone = 0.1f;
};

void action_bind_key(const std::string& name, int key, float scale = 1.0f);
void action_bind_mouse_button(const std::string& name, int button);
void action_bind_mouse_axis(const std::string& name, bool x_axis, 
                            float dead_zone = 0.1f, float scale = 1.0f);
void action_bind_gamepad_button(const std::string& name, int button);
void action_bind_gamepad_axis(const std::string& name, bool x_axis, 
                              float dead_zone = 0.1f, float scale = 1.0f);

bool action_pressed(const std::string& name);
bool action_just_pressed(const std::string& name);
bool action_just_released(const std::string& name);
float action_get_value(const std::string& name);

void action_map_update(Window* window);
