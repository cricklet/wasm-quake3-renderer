#include "scenario.h"
#include "resources.h"
#include "renderable.h"
#include "gl_helpers.h"
#include "bsp.h"
#include "pprint.hpp"

TextureRenderer::TextureRenderer(TextureRendererMode mode) {
  if (mode == TextureRendererMode::FLIP_VERTICALLY) {
    for (int i = 0; i < sizeof(_vertices) / sizeof(_vertices[0]); i += 4) {
      _vertices[i + 1] = - _vertices[i + 1];
    }
  }
}

void TextureRenderer::startLoading() {
  _shaderResourceID = ResourceManager::nextID();
  ResourceManager::getInstance()->loadShaders({
    "./src/glsl/test.vert",
    "./src/glsl/test.frag",
    _shaderResourceID
  });
}

bool TextureRenderer::finishLoading() {
  ////////////////////////////////////////////////////////////////////////////
  // Generate EBO
  glGenBuffers(1, &_ebo);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_elements), _elements, GL_STATIC_DRAW);

  ////////////////////////////////////////////////////////////////////////////
  // Generate VBO
  glGenBuffers(1, &_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);

  // Copy the vertex data into the vbo
  glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

  ////////////////////////////////////////////////////////////////////////////
  // Get the test shader program
  optional<GLuint> shaderProgram = ResourceManager::getInstance()->getShaderProgram(_shaderResourceID);
  if (!shaderProgram) {
    cerr << "failed to load shader program\n";
    return false;
  }

  // Use the program...
  glLinkProgram(*shaderProgram);
  glUseProgram(*shaderProgram);

  ////////////////////////////////////////////////////////////////////////////
  // Specify the inputs

  _inPosition = glGetAttribLocation(*shaderProgram, "inPosition");
  glEnableVertexAttribArray(_inPosition);

  _inTextureCoords = glGetAttribLocation(*shaderProgram, "inTextureCoords");
  glEnableVertexAttribArray(_inTextureCoords);

  _unifTexture = glGetUniformLocation(*shaderProgram, "unifTexture");

  if (hasErrors()) {
    return false;
  }

  return true;
}

void TextureRenderer::render(GLuint textureID) {
  optional<GLuint> shaderProgram = ResourceManager::getInstance()->getShaderProgram(_shaderResourceID);
  if (!shaderProgram) {
    cerr << "failed to load shader program\n";
    return;
  }

  glUseProgram(*shaderProgram);

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glUniform1i(_unifTexture, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);

  glVertexAttribPointer(_inPosition, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glVertexAttribPointer(_inTextureCoords, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));

  glDrawElements(GL_TRIANGLES, sizeof(_elements) / sizeof(_elements[0]), GL_UNSIGNED_INT, 0);

  hasErrors();
}

void TestScenario::startLoading() {
  _textureResourceID = ResourceManager::nextID();
  ResourceManager::getInstance()->loadResource({
    "./data/textures/poptart.jpg",
    ResourceType::IMAGE_FILE,
    _textureResourceID
  });

  _renderer.startLoading();
}

bool TestScenario::loadDependencies() {
  return false;
}

bool TestScenario::finishLoading() {
  if (!ResourceManager::getInstance()->finishedLoading()) {
    cerr << "tried to load scenario before resources finished loading\n";
    return false;
  }

  if (!_renderer.finishLoading()) {
    return false;
  }

  return true;
}

void TestScenario::render() {
  optional<GLuint> textureId = ResourceManager::getInstance()->getTexture(_textureResourceID);
  if (textureId) {
    _renderer.render(*textureId);
  }
}

void BSPScenario::startLoading() {
  _bspResourceID = ResourceManager::nextID();
  ResourceManager::getInstance()->loadResource({
    "./data/aerowalk.bsp",
    ResourceType::BSP_FILE,
    _bspResourceID
  });

  _sceneShaderResourceID = ResourceManager::nextID();
  ResourceManager::getInstance()->loadShaders({
    "./src/glsl/render_scene.vert",
    "./src/glsl/render_scene.frag",
    _sceneShaderResourceID
  });

  _compositingRenderer.startLoading();
}

bool BSPScenario::loadDependencies() {
  if (!_renderableMap) {
    ResourcePtr<const BSPMap> mapResource = ResourceManager::getInstance()->getMap();
    if (!mapResource.get()) {
      cerr << "map failed to load\n";
      return false;
    }
    _renderableMap = make_shared<RenderableBSP>(mapResource);
  }

  if (_renderableMap) {
    return _renderableMap->loadDependencies();
  }

  cerr << "failed to load dependencies\n";
  return false;
}

bool BSPScenario::finishLoading() {
  if (!ResourceManager::getInstance()->finishedLoading()) {
    cerr << "tried to load scenario before resources finished loading\n";
    return false;
  }

  if (loadDependencies()) {
    cerr << "tried to finish loading when continue loading is still dirty\n";
    return false;
  }

  {
    bool success = _compositingRenderer.finishLoading();
    if (!success) {
      return false;
    }
  }

  { // Finish loading the map. This generates textures, VBOs, & EBOs
    bool success = _renderableMap->finishLoading();
    if (!success) {
      return false;
    }
  }

  // Get the screen size, this will be used for FBOs later
  GLint viewportSize[4];
  glGetIntegerv(GL_VIEWPORT, viewportSize);
  int screenWidth = viewportSize[2];
  int screenHeight = viewportSize[3];

  // Load the shader
  _sceneShader = *ResourceManager::getInstance()->getShaderProgram(_sceneShaderResourceID);

  // Use the program...
  glLinkProgram(_sceneShader);
  glUseProgram(_sceneShader);

  // Bind the inputs
  _inPosition = glGetAttribLocation(_sceneShader, "inPosition");
  glEnableVertexAttribArray(_inPosition);

  _inTextureCoords = glGetAttribLocation(_sceneShader, "inTextureCoords");
  glEnableVertexAttribArray(_inTextureCoords);

  _inLightmapCoords = glGetAttribLocation(_sceneShader, "inLightmapCoords");
  glEnableVertexAttribArray(_inLightmapCoords);

  _inColor = glGetAttribLocation(_sceneShader, "inColor");
  glEnableVertexAttribArray(_inColor);

  _unifTexture = glGetUniformLocation(_sceneShader, "unifTexture");
  _unifLightmapTexture = glGetUniformLocation(_sceneShader, "unifLightmapTexture");
  _unifCameraTransform = glGetUniformLocation(_sceneShader, "unifCameraTransform");
  _unifProjTransform = glGetUniformLocation(_sceneShader, "unifProjTransform");

  // Create an FBO for solid elements
  glGenFramebuffers(1, &_sceneFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, _sceneFBO);
  {
    // Create a color texture for the FBO
    glGenTextures(1, &_sceneTexture);
    glBindTexture(GL_TEXTURE_2D, _sceneTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _sceneTexture, 0);

    // Create a depth texture for the FBO
    glGenTextures(1, &_sceneDepthTexture);
    glBindTexture(GL_TEXTURE_2D, _sceneDepthTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, screenWidth, screenHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _sceneDepthTexture, 0);
  }

  // Create an FBO for translucent elements
  glGenFramebuffers(1, &_effectsFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, _effectsFBO);
  {
    // Create a color texture for the FBO
    glGenTextures(1, &_effectsTexture);
    glBindTexture(GL_TEXTURE_2D, _effectsTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _effectsTexture, 0);
  }

  if (hasErrors()) {
    return false;
  }

  return true;
}

glm::vec3 Camera::forward() {
  return glm::vec3(
    sin(pitch) * cos(-yaw),
    sin(pitch) * sin(-yaw),
    -cos(pitch)
  );
}

glm::vec3 Camera::right() {
  return glm::cross(forward(), upApproximate());
}

glm::vec3 Camera::upApproximate() {
  return glm::vec3(0,0,-1);
}

void BSPScenario::think(glm::vec2 dir, double pitch, double yaw) {
  _camera.pitch = pitch;
  _camera.yaw = yaw;
  _camera.location += _camera.forward() * (dir.y * 10.0f);
  _camera.location += _camera.right() * (dir.x * 10.0f);
}

void BSPScenario::render() {
  ResourcePtr<const BSPMap> mapResource = ResourceManager::getInstance()->getMap();
  const BSPMap* map = mapResource.get();

  //////////////////////////////////////////////////////////////////////////////

  optional<GLuint> sceneShaderID = ResourceManager::getInstance()->getShaderProgram(_sceneShaderResourceID);
  if (!sceneShaderID) {
    cerr << "failed to load shader program\n";
    return;
  }

  if (!_renderableMap) {
    cerr << "failed to load map\n";
    return;
  }

  // Render all the solid geometry in the map to the scene-FBO
  {
    glUseProgram(_sceneShader);
    glBindFramebuffer(GL_FRAMEBUFFER, _sceneFBO);

    glClearColor(0.6, 0.2, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    // glDepthMask(GL_TRUE);
    // glDisable(GL_BLEND);

    // Update camera transform
    glm::mat4 cameraTransform = glm::lookAt(
      _camera.location, // location of camera
      _camera.location + _camera.forward(), // look at
      glm::vec3(0,0,1)  // camera up vector
    );

    glUniformMatrix4fv(_unifCameraTransform, 1, GL_FALSE, glm::value_ptr(cameraTransform));

    // And projection transform
    glm::mat4 projectionTransform = glm::perspective(glm::radians(90.0f), 1200.0f / 800.0f, 0.5f, 10000.0f);
    glUniformMatrix4fv(_unifProjTransform, 1, GL_FALSE, glm::value_ptr(projectionTransform));

    ShaderParameters shaderInputs {
      _inPosition,
      _inColor,
      _inTextureCoords,
      _inLightmapCoords,
      _unifTexture,
      _unifLightmapTexture,
      RenderMode::SOLID
    };
    _renderableMap->render(shaderInputs);
  }

  // // Render all the transparent geometry in the map to the effects-scene-FBO
  // glBindFramebuffer(GL_FRAMEBUFFER, _effectsFBO);

  // glDepthMask(GL_FALSE);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // {
  //   ShaderParameters shaderInputs {
  //     _inPosition,
  //     _inColor,
  //     _inTextureCoords,
  //     _inLightmapCoords,
  //     _unifTexture,
  //     _unifLightmapTexture,
  //     RenderMode::TRANSPARENCY
  //   };
  //   _renderableMap->render(shaderInputs);
  // }

  // Composite them onto the screen
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  _compositingRenderer.render(_sceneTexture);
}
