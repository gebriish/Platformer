#include <GLFW/glfw3.h>
#include "engine/log.hpp"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int main() {
  GLFWwindow* window;

  if(!glfwInit())
    return -1;

  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Platformer", NULL, NULL);
  log_info("window created! [%.1f, %.1f]", SCREEN_WIDTH, SCREEN_HEIGHT);
 
  // Window Creation Error Handling
  if (!window)
  {
    glfwTerminate();
    return -1;
  }
  
  glfwMakeContextCurrent(window);

  while(!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  log_warn("app exit()");
  return 0;
}
