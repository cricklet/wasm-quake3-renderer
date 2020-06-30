
#include "support.h"
#include "bsp.h"
#include "gl_helpers.h"

using BSPMap = BSP::header_t;
const BSPMap* currentMap = nullptr;

bool started = false;

int TEST_SHADER = 0;
unordered_map<int, GLuint> shaderPrograms = {};

optional<std::function<void()>> currentRenderer;

///////////////////////////////////////////////////////////////////////////////
// Working through open.gl again...

optional<std::function<void()>> generateTestShader() {
  static float vertices[] = {
    0.0f,  0.5f, // Vertex 1 (X, Y)
    0.5f, -0.5f, // Vertex 2 (X, Y)
    -0.5f, -0.5f  // Vertex 3 (X, Y)
  };

  // And save the following attribute configuration as a vao (vertex array object)
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

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

  // Make sure we render the "outColor" from the program
  // glBindFragDataLocation(program, 0, "outColor"); /// ??????

  // Use the program...
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  // Specify the layout of the vertices
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(posAttrib);

  return [=]() {
    glClearColor(0.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);
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
    optional<GLuint> shaderProgram = compileShaderProgram((const char*) vert, (const char*) frag);
    if (shaderProgram) {
      cout << "adding shader program for " << shaderID << "\n";
      shaderPrograms[shaderID] = *shaderProgram;
      return true;
    }
    cerr << "failed to create shader program\n";
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
// => JS

extern "C" {
  extern void testJS();
}

///////////////////////////////////////////////////////////////////////////////

std::function<void()> loop;
void mainLoop() {
  loop();
}

int main() {
  testJS();

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