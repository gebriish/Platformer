#include "../core.hpp"

#include <GLFW/glfw3.h>
#include <cstdlib>

//======================================================================//
//                   GLOBAL CORE APPLICATION STATE                      //
//======================================================================//
static struct {
  float frame_begin_time;
  float delta_time;
} g_CoreState;

void core_init() {
  if(!glfwInit()) {
    exit(0);
  }
}

void core_update() {
  float begin_time = (float) glfwGetTime();
  g_CoreState.delta_time = begin_time - g_CoreState.frame_begin_time;
  g_CoreState.frame_begin_time = begin_time;
}

void core_destroy() {
  glfwTerminate();
}

float core_time_seconds() {
  return (float) glfwGetTime();
}

float core_deltatime() {
  return g_CoreState.delta_time;
}
