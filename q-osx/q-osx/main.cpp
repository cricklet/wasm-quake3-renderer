#include "support.h"
#include "osx_helpers.h"
#include "app.h"


GLFWwindow* window;

std::function<void()> loopCallback;
static void loop(webview_t webview, void * t) {
  loopCallback();
}

int main(int argc, const char * argv[]) {
  // Setup GL window
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); // Windowed
  glfwMakeContextCurrent(window);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  OSXWebView::setupInstance("http://0.0.0.0:8000/osx.html");

  printf("OpenGL version supported by this platform : %s\n", glGetString(GL_VERSION));

  App app;
  loopCallback = [&] {
    if (!glfwWindowShouldClose(window)) {
      app.loop(window);
    }
  };
  OSXWebView::getInstance()->run(loop);

  return EXIT_SUCCESS;
}
