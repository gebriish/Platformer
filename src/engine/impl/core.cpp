#include "../core.hpp"

#include <GLFW/glfw3.h>
#include <cstdlib>

static struct {

} g_CoreState;

void core_init() {
  if(!glfwInit()) {
    exit(0);
  }
}

void core_destroy() {
  glfwTerminate();
}
