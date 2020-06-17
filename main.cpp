#include <functional>
#include <stdlib.h>
#include <emscripten.h>
#include <SDL.h>

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>

///////////////////////////////////////////////////////////////////////////////
// => C++

extern "C" {
  void EMSCRIPTEN_KEEPALIVE onClick() { printf("onClick\n"); }

  EMSCRIPTEN_KEEPALIVE void* createBuffer(int bytes) {
    return malloc(bytes * sizeof(char));
  }

  EMSCRIPTEN_KEEPALIVE void destroyBuffer(void* pointer) {
    free(pointer);
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
  SDL_CreateWindowAndRenderer(640, 480, 0, &window, nullptr);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  loop = [&] {
    SDL_GL_SwapWindow(window);
  };

  emscripten_set_main_loop(mainLoop, 0, true);

  return EXIT_SUCCESS;
}