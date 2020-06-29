#ifndef SUPPORT_H
#define SUPPORT_H

#include <emscripten.h>
#include <emscripten/html5.h>

// Window, renderer, input
#include <SDL2/SDL.h>

// OpenGL extensions
// #define GLEW_STATIC
// #include <GL/glew.h>
// #include <SDL2/SDL_opengles2.h>
#include <GLES3/gl3.h>

#include <stdlib.h>
#include <iostream>
#include <functional>
#include <optional>
#include <unordered_map>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

#endif