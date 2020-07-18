#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "support.h"

struct VBO;
struct EBO;
namespace BSP {
  struct header_t;
}
using BSPMap = BSP::header_t;

struct RenderableBSP {
  RenderableBSP(ResourcePtr<const BSPMap> map): _map(map) {}

  bool loadDependencies();
  void generateBuffersForRendering();
  
  ResourcePtr<const BSPMap> _map;

  unordered_map<string, int> _textureResourceIds;
};

#endif