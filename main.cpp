
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
  GLuint program = shaderPrograms.at(TEST_SHADER);

  // Make sure we render the "outColor" from the program
  glBindFragDataLocationIndexed(program, 0, 0, "outColor"); /// ??????

  // And use our vertices VBO above to input into "position"
  GLint posAttrib = glGetAttribLocation(program, "position");
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(posAttrib);

  // And save this attribute configuration as a vao (vertex array object)
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  return [=]() {
    glClearColor ( 0.0, 1.0, 1.0, 1.0 );
    glClear ( GL_COLOR_BUFFER_BIT );

    glUseProgram(program);
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

  EmscriptenWebGLContextAttributes attr;
  emscripten_webgl_init_context_attributes(&attr);
  attr.majorVersion = 3;
  attr.minorVersion = 0;
  EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("#canvas", &attr);

  emscripten_webgl_make_context_current(ctx);

  printf("OpenGL version supported by this platform : %s\n", glGetString(GL_VERSION));

  loop = [&] {
    if (!started) {
      return;
    }

    glClearColor ( 0.0, 0.0, 1.0, 1.0 );
    glClear ( GL_COLOR_BUFFER_BIT );

    if (currentRenderer) {
      printf("qwert\n");
      (*currentRenderer)();
    }
  };

  emscripten_set_main_loop(mainLoop, 0, true);

  return EXIT_SUCCESS;
}