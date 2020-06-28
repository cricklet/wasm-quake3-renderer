#include <functional>
#include <stdlib.h>
#include <emscripten.h>
#include <SDL.h>

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>

namespace BSP {
  typedef struct {
    int offset;
    int length;
  } direntry_t;

  typedef struct {
    char magic[4];
    int version;
    direntry_t direntries[17];
  } header_t;

  void debugString(const BSP::header_t* header) {
    printf("map {\n");
    printf(" magic: %s\n", header->magic);
    printf(" version: %d\n", header->version);
    for (const direntry_t& entry : header->direntries) {
      printf(" entry offset: %d, size: %d\n", entry.offset, entry.length);
    }
    printf("}\n");
  }
};

using BSPMap = BSP::header_t;
const BSPMap* currentMap = nullptr;

bool started = false;

///////////////////////////////////////////////////////////////////////////////
// => C++

extern "C" {
  void EMSCRIPTEN_KEEPALIVE CPP_onClick() { printf("onClick\n"); }

  EMSCRIPTEN_KEEPALIVE void* CPP_createBuffer(int bytes) {
    printf("malloc for %d bytes\n", bytes);
    return malloc(bytes * sizeof(char));
  }

  EMSCRIPTEN_KEEPALIVE void CPP_destroyBuffer(void* pointer) {
    free(pointer);
  }

  EMSCRIPTEN_KEEPALIVE void CPP_setCurrentMap(void* pointer) {
    currentMap = (const BSPMap*) pointer;
  }

  EMSCRIPTEN_KEEPALIVE void CPP_start() {
    started = true;
    BSP::debugString(currentMap);
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
    if (!started) {
      return;
    }

    SDL_GL_SwapWindow(window);
  };

  emscripten_set_main_loop(mainLoop, 0, true);

  return EXIT_SUCCESS;
}