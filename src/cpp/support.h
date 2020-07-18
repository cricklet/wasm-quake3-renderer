#ifndef SUPPORT_H
#define SUPPORT_H

#include <emscripten.h>
#include <emscripten/html5.h>

// Window, renderer, input
// #include <SDL2/SDL.h>
#include <GLFW/glfw3.h>

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
#include <unordered_set>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define warn cout << string(__FILE__) << ":" << to_string(__LINE__) << " "

template<typename K, typename V>
const V* getValue(const std::unordered_map<K, V>& map, K key) {
  if (map.count(key)) {
    return &map.at(key);
  }
  return nullptr;
}

using namespace std;

template<typename T>
struct ResourcePtr {
  ResourcePtr(T* pointer): _pointer(pointer) {
    _counter = shared_ptr<int>(0);
    (*_counter) ++;
  }

  ResourcePtr(ResourcePtr<T>& other) { 
    _pointer = other._pointer;
    _counter = other._counter;
    (*_counter)++; 
  }

  T* get() { return _pointer; }
  T& operator*() { return *_pointer; }
  T* operator->() { return _pointer; }

  ~ResourcePtr() {
    (*_counter) --;
    if (*_counter == 0) {
      free((void*) _pointer);
    }
  }

private:
  T* _pointer = nullptr;
  shared_ptr<int> _counter = nullptr;
};

#endif