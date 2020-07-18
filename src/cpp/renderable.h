#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "support.h"
#include "gl_helpers.h"

namespace BSP {
  struct header_t;
  struct face_t;
}
using BSPMap = BSP::header_t;

struct RenderableFace {
  static optional<RenderableFace> generate(const BSPMap* map, int faceIndex);
  int faceIndex; // auto* face = map->faces() + faceIndex
  VBO vertices;
  VBO colors;
  EBO elements;
};

struct RenderableBSP {
  RenderableBSP(ResourcePtr<const BSPMap> map): _map(map) {}

  bool loadDependencies();
  bool finishLoading();

  struct ShaderParameters {
    GLuint inPosition;
    GLuint inColor;
    GLuint inTextureCoords;
    GLuint inLightmapCoords;

    GLuint unifTexture;
    GLuint unifLightmapTexture;
  };
  void render(const ShaderParameters& inputs);

private:
  ResourcePtr<const BSPMap> _map;
  unordered_map<string, int> _textureResourceIds;

  unordered_map<int, GLuint> _lightmapTextures;
  GLuint _fallbackLightmapTexture;

  vector<RenderableFace> _renderableFaces;
};

#endif