
#include "support.h"
#include "bsp.h"
#include "gl_helpers.h"
#include "messages.h"
#include "bindings.h"

using BSPMap = BSP::header_t;
const BSPMap* currentMap = nullptr;

bool started = false;

int TEST_SHADER = 0;
unordered_map<int, GLuint> shaderPrograms = {};

int CAT_TEXTURE = 0;
unordered_map<int, GLuint> textures = {};

optional<std::function<void()>> currentRenderer;

///////////////////////////////////////////////////////////////////////////////
// Working through open.gl again...

optional<std::function<void()>> generateTestShader() {
  float vertices[] = {
    // Position   Texcoords
    -0.5f,  0.5f, 0.0f, 0.0f, // Top-left
     0.5f,  0.5f, 1.0f, 0.0f, // Top-right
     0.5f, -0.5f, 1.0f, 1.0f, // Bottom-right
    -0.5f, -0.5f, 0.0f, 1.0f  // Bottom-left
  };
  GLuint elements[] = {
    0, 1, 2,
    2, 3, 0
  };

  // And save the following attribute configuration as a vao (vertex array object)
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  ////////////////////////////////////////////////////////////////////////////
  // Generate EBO
  GLuint ebo;
  glGenBuffers(1, &ebo);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

  ////////////////////////////////////////////////////////////////////////////
  // Generate VBO
  GLuint vbo;
  glGenBuffers(1, &vbo);

  // Choose the vbo...
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // Copy the vertex data into the vbo
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  ////////////////////////////////////////////////////////////////////////////
  // Get the test shader program
  if (shaderPrograms.count(TEST_SHADER) == 0) {
    cout << "shader missing\n";
    return {};
  }
  GLuint shaderProgram = shaderPrograms.at(TEST_SHADER);

  // Use the program...
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  ////////////////////////////////////////////////////////////////////////////
  // Specify the inputs

  // For the vertices
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glEnableVertexAttribArray(posAttrib);

  // And colors
  GLint texAttrib = glGetAttribLocation(shaderProgram, "texCoord");
  glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));
  glEnableVertexAttribArray(texAttrib);

  return [=]() {
    glClearColor(0.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, sizeof(elements) / sizeof(elements[0]), GL_UNSIGNED_INT, 0);

    hasErrors();
  };
}

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

  EMSCRIPTEN_KEEPALIVE void CPP_setCurrentMap(void* pointer) {
    currentMap = (const BSPMap*) pointer;
  }

  EMSCRIPTEN_KEEPALIVE bool CPP_createShaderProgram(int shaderID, const void* vert, const void* frag) {
    cout << "starting to compile shaders for " << shaderID << "\n";
    optional<GLuint> shaderProgram = GLHelpers::compileShaderProgram((const char*) vert, (const char*) frag);
    if (shaderProgram) {
      cout << "adding shader program for " << shaderID << "\n";
      shaderPrograms[shaderID] = *shaderProgram;
      return true;
    }
    cerr << "failed to create shader program\n";
    return false;
  }

  EMSCRIPTEN_KEEPALIVE bool CPP_createTexture(int textureID, const void* image, int width, int height) {
    optional<GLuint> tex = GLHelpers::loadTexture(image, width, height);
    if (tex) {
      textures[textureID] = *tex;
      return true;
    }

    return false;
  }

  EMSCRIPTEN_KEEPALIVE void CPP_start() {
    started = true;
    BSP::debugString(currentMap);

    // Setup shader!
    currentRenderer = generateTestShader();
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

std::function<void()> loop;
void mainLoop() {
  loop();
}

int main() {
  MessagesFromWeb::getInstance()->registerHandler(messageLogger);
  MessageBindings::sendMessageToWeb(CPPLoaded{});
  MessageBindings::sendMessageToWeb(TestMessage{ "main() called in CPP" });

  MessageBindings::sendMessageToWeb(LoadResource{
    "./data/aerowalk.bsp",
    ResourceType::BSP_FILE,
    0
  });

  MessageBindings::sendMessageToWeb(LoadResource{
    "./data/poptart.jpg",
    ResourceType::IMAGE_FILE,
    1
  });

  MessageBindings::sendMessageToWeb(LoadShaders{
    "./shader/test.vert",
    "./shader/test.frag",
    2
  });

  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); // Windowed

  glfwMakeContextCurrent(window);

  printf("OpenGL version supported by this platform : %s\n", glGetString(GL_VERSION));

  loop = [&] {
    if (!started) {
      return;
    }

    if (currentRenderer) {
      (*currentRenderer)();
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  };

  emscripten_set_main_loop(mainLoop, 0, true);

  return EXIT_SUCCESS;
}