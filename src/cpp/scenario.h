#ifndef SCENARIO_H
#define SCENARIO_H

#include "support.h"
#include "resources.h"
#include "gl_helpers.h"

struct IScenario : IHasResources {
public:
  virtual ~IScenario() {}
  virtual void think(glm::vec2 dir, double pitch, double yaw) = 0;
  virtual void render() = 0;
};

enum class TextureRendererMode {
  DEFAULT, FLIP_VERTICALLY
};

struct TextureRenderer : IHasResources {
  TextureRenderer(TextureRendererMode mode = TextureRendererMode::DEFAULT);

  void render(vector<GLuint> textureIDs);

private:
  bool finishLoading() override;

  int _shaderResourceID;

  GLuint _vao;
  GLuint _vbo;
  GLuint _ebo;

  GLuint _inPosition;
  GLuint _inTextureCoords;

  GLuint _unifTexture;

  float _vertices[16] = {
    // Position   Texcoords
    -1.0f,  1.0f, 0.0f, 0.0f, // Top-left
    1.0f,  1.0f, 1.0f, 0.0f, // Top-right
    1.0f, -1.0f, 1.0f, 1.0f, // Bottom-right
    -1.0f, -1.0f, 0.0f, 1.0f  // Bottom-left
  };

  GLuint _elements[6] = {
    0, 1, 2,
    2, 3, 0
  };
};

struct TestScenario : IScenario {
public:
  TestScenario();
  void think(glm::vec2 dir, double pitch, double yaw) override {}
  void render() override;

private:
  bool finishLoading() override;
};

struct PopTartScenario : IScenario {
public:
  PopTartScenario();
  void think(glm::vec2 dir, double pitch, double yaw) override {}
  void render() override;

private:
  bool finishLoading() override;

  int _textureResourceID;
  shared_ptr<TextureRenderer> _renderer = nullptr;
};

struct Camera {
public:
  glm::vec3 location;
  double pitch;
  double yaw;

  glm::vec3 forward();
  glm::vec3 right();
  glm::vec3 upApproximate();
};

struct RenderableBSP;

struct SceneShaderParameters {
  GLuint inPosition;
  GLuint inColor;
  GLuint inTextureCoords;
  GLuint inLightmapCoords;

  GLuint unifHighlight;
  GLuint unifAlpha;
  GLuint unifTexture;
  GLuint unifLightmapTexture;

  GLuint unifCameraTransform;
  GLuint unifProjTransform;
};

struct BSPScenario : IScenario {
public:
  BSPScenario();
  void think(glm::vec2 dir, double pitch, double yaw) override;
  void render() override;

private:
  bool finishLoading() override;

  int _bspResourceID;
  int _sceneShaderResourceID;

  shared_ptr<TextureRenderer> _compositingRenderer = nullptr;
  shared_ptr<RenderableBSP> _renderableMap = nullptr;

  unordered_map<int, GLuint> _lightmapTextures;
  GLuint _fallbackLightmapTexture;

  GLuint _sceneShader;
  SceneShaderParameters _sceneShaderParams;

  GLuint _vao;
  
  // For rendering with the scene shader
  vector<VBO> _verticesPerFace;
  vector<VBO> _colorsPerFace;
  vector<EBO> _elementsPerFace;

  // For rendering solid elements
  GLuint _sceneFBO;
  GLuint _sceneTexture;
  GLuint _sceneDepthTexture;

  // For rendering transparency
  GLuint _effectsFBO;
  GLuint _effectsTexture;

  // For rendering FBOs to the screen
  VBO _screenVBO;
  GLuint _screenShader;
  
  // For testing
  int _poptartResourceID;

  Camera _camera;
};

#endif
