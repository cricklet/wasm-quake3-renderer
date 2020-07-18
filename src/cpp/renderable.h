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

  vector<VBO> _verticesPerFace;
  vector<VBO> _colorsPerFace;
  vector<EBO> _elementsPerFace;
};

#endif