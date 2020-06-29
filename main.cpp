
#include "support.h"
#include "bsp.h"
#include "gl_helpers.h"

using BSPMap = BSP::header_t;
const BSPMap* currentMap = nullptr;

bool started = false;

unordered_map<string, GLuint> shaderPrograms = {};

std::function<void()> renderShader;

///////////////////////////////////////////////////////////////////////////////
// Working through open.gl again...

std::function<void()> generateTestShader() {
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
  GLuint program = shaderPrograms.at("test");

  // // Make sure we render the "outColor" from the program
  // glBindFragDataLocationIndexed(program, 0, 0, "outColor");

  // And use our vertices VBO above to input into "position"
  GLint posAttrib = glGetAttribLocation(program, "position");
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(posAttrib);

  // And save this attribute configuration as a vao (vertex array object)
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  return [&]() {
    glUseProgram(program);
    glDrawArrays(GL_TRIANGLES, 0, 3);
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

  EMSCRIPTEN_KEEPALIVE bool CPP_createShaderProgram(const char* name, const void* vert, const void* frag) {
    optional<GLuint> shaderProgram = compileShaderProgram((const char*) vert, (const char*) frag);
    if (shaderProgram) {
      shaderPrograms[name] = *shaderProgram;
      return true;
    }
    return false;
  }

  EMSCRIPTEN_KEEPALIVE void CPP_start() {
    started = true;
    BSP::debugString(currentMap);

    // Setup shader!
    renderShader = generateTestShader();
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

  SDL_Window *window;
  SDL_Renderer *renderer = NULL;
  SDL_CreateWindowAndRenderer(512, 512, SDL_WINDOW_OPENGL, &window, &renderer);

  // glewExperimental = GL_TRUE;
  // if (GLEW_OK != glewInit()) {
  //   cerr << "Error setting up glew\n";
  // }

  printf("OpenGL version supported by this platform : %s\n", glGetString(GL_VERSION));

  loop = [&] {
    if (!started) {
      return;
    }

    // renderShader();
    SDL_GL_SwapWindow(window);
  };

  emscripten_set_main_loop(mainLoop, 0, true);

  return EXIT_SUCCESS;
}