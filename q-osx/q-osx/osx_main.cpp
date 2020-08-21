#include "support.h"
#include "osx_helpers.h"
#include "bindings.h"
#include "binding_helpers.h"
#include "app.h"


GLFWwindow* window;

std::function<void()> loopCallback;
static void loop(webview_t webview, void* arg) {
  OSXWebView::getInstance()->queueDispatch(loop);
  loopCallback();
}

shared_ptr<App> app = nullptr;


struct AppStarter : IMessageHandler {
public:
  void handleMessageFromWeb(const OSXReady& message) override {
    cout << "loaded webview. let's start the app!\n";
    app = shared_ptr<App>{ new App() };
  }
};

int main(int argc, const char * argv[]) {
  // Setup GL window
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); // Windowed
  glfwMakeContextCurrent(window);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glewExperimental = GL_TRUE;
  glewInit();

  OSXWebView::setupInstance("http://0.0.0.0:8000/osx.html");
  
  printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
  printf("OpenGL version supported by this platform : %s\n", glGetString(GL_VERSION));

  shared_ptr<AppStarter> appStarter { new AppStarter() };
  MessagesFromWeb::getInstance()->registerHandler(appStarter);

  loopCallback = [&] {
    if (glfwWindowShouldClose(window)) {
      return;
    }
    if (app) {
      app->loop(window);
    }
  };

  OSXWebView::getInstance()->queueDispatch(loop);
  OSXWebView::getInstance()->run();

  return EXIT_SUCCESS;
}
