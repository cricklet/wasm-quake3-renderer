
#include "support.h"
#include "bsp.h"
#include "gl_helpers.h"
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

struct MessageLogger : IMessageHandler {
public:
  MessageLogger() {}
  ~MessageLogger() {}

  void handleMessageFromWeb(const TestMessage& message) {
    cout << "received test message w/ text: " << message.text << "\n";
  }
};

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
  
  currentScenario = make_shared<TestScenario>();
  currentScenario->startLoading();

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); // Windowed
  glfwMakeContextCurrent(window);

  printf("OpenGL version supported by this platform : %s\n", glGetString(GL_VERSION));

  loopCallback = [&] {
    if (!ResourceManager::getInstance()->finishedLoading()) {
      return;
    }

    static bool firstTime = true;
    if (firstTime) {
      firstTime = false;
      currentScenario->finishLoading();
    }

    currentScenario->think();
    currentScenario->render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  };

  emscripten_set_main_loop(emscriptenLoop, 0, true);

  return EXIT_SUCCESS;
}