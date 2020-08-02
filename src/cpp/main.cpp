
#include "support.h"
#include "messages.h"
#include "bindings.h"
#include "resources.h"
#include "scenario.h"


///////////////////////////////////////////////////////////////////////////////
// => C++

extern "C" {
  void EMSCRIPTEN_KEEPALIVE CPP_onClick() { cout << "onClick\n"; }

  EMSCRIPTEN_KEEPALIVE void* CPP_createBuffer(int bytes) {
    cout << "malloc for " <<  bytes << " bytes\n";
    return malloc(bytes * sizeof(char));
  }

  EMSCRIPTEN_KEEPALIVE void CPP_destroyBuffer(void* pointer) {
    free(pointer);
  }
}

///////////////////////////////////////////////////////////////////////////////

shared_ptr<MessageLogger> messageLogger = make_shared<MessageLogger>();

///////////////////////////////////////////////////////////////////////////////

shared_ptr<IScenario> currentScenario;

std::function<void()> loopCallback;
void emscriptenLoop() {
  loopCallback();
}

int main() {
  MessagesFromWeb::getInstance()->registerHandler(messageLogger);
  MessagesFromWeb::getInstance()->registerHandler(ResourceManager::getInstance());

  MessageBindings::sendMessageToWeb(CPPLoaded{});
  MessageBindings::sendMessageToWeb(TestMessage{ "main() called in CPP" });
  
  // currentScenario = make_shared<TestScenario>();
  currentScenario = make_shared<BSPScenario>();
  currentScenario->startLoading();

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

  loopCallback = [&] {
    static bool error = false;
    if (hasErrors() || error) {
      error = true;
      return;
    }

    if (!ResourceManager::getInstance()->finishedLoading()) {
      return;
    }

    if (currentScenario->loadDependencies()) {
      cout << "needs to continue loading secondary resources\n";
      return;
    }

    static bool firstTime = true;
    static bool quit = false;
    if (firstTime) {
      firstTime = false;
      if (!currentScenario->finishLoading()) {
        quit = true;
      }
    }

    if (quit) {
      return;
    }

    glm::vec2 dir = {0, 0};
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { dir += glm::vec2(0.0, 1.0); }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { dir += glm::vec2(1.0, 0.0); }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { dir += glm::vec2(-1.0, 0.0); }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { dir += glm::vec2(0.0, -1.0); }

    if (glm::length(dir) > 0) {
      dir = glm::normalize(dir);
    }

    static optional<double> oldX, oldY;
    static double pitch = - 3.14159 * 0.5;
    static double yaw = 0;

    double x, y;
    glfwGetCursorPos(window, &x, &y);

    if (oldX && oldY) {
      double dx = x - *oldX;
      double dy = y - *oldY;
      pitch += dy / 400.0f;
      yaw +=  dx / 400.0f;
      if(pitch > 89.0) pitch =  89.0;
      if(pitch < -89.0) pitch = -89.0;
    }
    oldX = x;
    oldY = y;

    currentScenario->think(dir, pitch, yaw);
    currentScenario->render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  };

  emscripten_set_main_loop(emscriptenLoop, 0, true);

  return EXIT_SUCCESS;
}