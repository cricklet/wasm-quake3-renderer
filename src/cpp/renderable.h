#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "support.h"
#include "gl_helpers.h"
#include "resources.h"

namespace BSP {
  struct header_t;
  struct face_t;
}
using BSPMap = BSP::header_t;

struct SceneShaderParameters;
struct HitScanResult;

struct RenderableFace {
  static optional<RenderableFace> generate(const BSPMap* map, int faceIndex);
  int faceIndex; // auto* face = map->faces() + faceIndex
  VBO vertices;
  VBO colors;
  EBO elements;
};

enum class RenderMode {
  SOLID,
  TRANSPARENCY
};

struct RenderableBSP : IHasResources {
  RenderableBSP(ResourcePtr<const BSPMap> map);

  void render(const SceneShaderParameters& inputs, RenderMode mode, const optional<HitScanResult>& hitScanResult);

private:
  bool finishLoading() override;

  ResourcePtr<const BSPMap> _map;
  unordered_map<string, int> _textureResourceIds;

  unordered_map<int, GLuint> _lightmapTextures;
  GLuint _fallbackLightmapTexture;

  vector<RenderableFace> _renderableFaces;
};

#endif