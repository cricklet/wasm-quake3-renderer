#include "support.h"
#include "webview.h"

int main(int argc, const char * argv[]) {
  // Setup GL window
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); // Windowed
  glfwMakeContextCurrent(window);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  
  
  // Setup the webview
  webview::webview w(true, nullptr);
  w.set_title("q");
  w.set_size(300, 600, WEBVIEW_HINT_FIXED);
  w.navigate("http://0.0.0.0:8000/osx.html");
  w.run();
  

  printf("OpenGL version supported by this platform : %s\n", glGetString(GL_VERSION));

  while(!glfwWindowShouldClose(window)) {
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  return EXIT_SUCCESS;
}
