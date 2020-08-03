#ifdef __APPLE__
#else

#include "support.h"
#include "app.h"

std::function<void()> loopCallback;
void emscriptenLoop() {
  loopCallback();
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); // Windowed
  glfwMakeContextCurrent(window);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  printf("OpenGL version supported by this platform : %s\n", glGetString(GL_VERSION));

  App app;
  loopCallback = [&] {
    app.loop(window);
  };

  emscripten_set_main_loop(emscriptenLoop, 0, true);

  return EXIT_SUCCESS;
}

#endif
