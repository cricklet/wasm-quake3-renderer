#include "scenario.h"
#include "resources.h"
#include "renderable.h"
#include "gl_helpers.h"
#include "bsp.h"
#include "hitscan.h"
#include "pprint.hpp"

TextureRenderer::TextureRenderer(TextureRendererMode mode) {
  if (mode == TextureRendererMode::FLIP_VERTICALLY) {
    for (int i = 0; i < sizeof(_vertices) / sizeof(_vertices[0]); i += 4) {
      _vertices[i + 1] = - _vertices[i + 1];
    }
  }

  cout << "starting to load TextureRenderer\n";
  _shaderResourceID = ResourceManager::nextID();
  ResourceManager::getInstance()->loadShaders(this, {
    "./src/glsl/test.vert",
    "./src/glsl/test.frag",
    _shaderResourceID
  });
}

bool TextureRenderer::finishLoading() {
  cout << "TextureRenderer::finishLoading\n";
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
    cerr << "failed to load shader program in TextureRenderer::load\n";
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

void TextureRenderer::render(vector<GLuint> textureIDs) {
  optional<GLuint> shaderProgram = ResourceManager::getInstance()->getShaderProgram(_shaderResourceID);
  if (!shaderProgram) {
    cerr << "failed to load shader program in TextureRenderer::render\n";
    return;
  }

  glUseProgram(*shaderProgram);

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glBlendFunc(GL_ONE, GL_ONE);

  for (const auto textureID : textureIDs) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(_unifTexture, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glVertexAttribPointer(_inPosition, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glVertexAttribPointer(_inTextureCoords, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));

    glDrawElements(GL_TRIANGLES, sizeof(_elements) / sizeof(_elements[0]), GL_UNSIGNED_INT, 0);
  }

  hasErrors();
}

TestScenario::TestScenario() {
  _textureResourceID = ResourceManager::nextID();
  ResourceManager::getInstance()->loadResource(this, {
    "./data/textures/poptart.jpg",
    ResourceType::IMAGE_FILE,
    _textureResourceID
  });

  cout << "starting TestScenario\n";
  _renderer = shared_ptr<TextureRenderer>(new TextureRenderer());
}

bool TestScenario::finishLoading() {
  return true;
}

void TestScenario::render() {
  optional<GLuint> textureId = ResourceManager::getInstance()->getTexture(_textureResourceID);
  if (textureId) {
    _renderer->render({*textureId});
  }
}

BSPScenario::BSPScenario() {
  _bspResourceID = ResourceManager::nextID();
  ResourceManager::getInstance()->loadResource(this, {
    "./data/aerowalk.bsp",
    ResourceType::BSP_FILE,
    _bspResourceID
  });

  _sceneShaderResourceID = ResourceManager::nextID();
  ResourceManager::getInstance()->loadShaders(this, {
    "./src/glsl/render_scene.vert",
    "./src/glsl/render_scene.frag",
    _sceneShaderResourceID
  });

  // The compositing renderer registers itself with the ResourceManager and owns it's own
  // loading flow.
  _compositingRenderer = make_shared<TextureRenderer>(TextureRendererMode::FLIP_VERTICALLY);
}

bool BSPScenario::finishLoading() {
  ResourcePtr<const BSPMap> mapResource = ResourceManager::getInstance()->getMap();
  if (!mapResource.get()) {
    cerr << "map failed to load\n";
    return false;
  }

  // The renderable map registers itself with the ResourceManager and owns it's own
  // loading flow.
  _renderableMap = make_shared<RenderableBSP>(mapResource);

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
  _sceneShaderParams.inPosition = glGetAttribLocation(_sceneShader, "inPosition");
  glEnableVertexAttribArray(_sceneShaderParams.inPosition);

  _sceneShaderParams.inTextureCoords = glGetAttribLocation(_sceneShader, "inTextureCoords");
  glEnableVertexAttribArray(_sceneShaderParams.inTextureCoords);

  _sceneShaderParams.inLightmapCoords = glGetAttribLocation(_sceneShader, "inLightmapCoords");
  glEnableVertexAttribArray(_sceneShaderParams.inLightmapCoords);

  _sceneShaderParams.inColor = glGetAttribLocation(_sceneShader, "inColor");
  glEnableVertexAttribArray(_sceneShaderParams.inColor);

  _sceneShaderParams.unifAlpha = glGetUniformLocation(_sceneShader, "unifAlpha");
  _sceneShaderParams.unifHighlight = glGetUniformLocation(_sceneShader, "unifHighlight");
  _sceneShaderParams.unifTexture = glGetUniformLocation(_sceneShader, "unifTexture");
  _sceneShaderParams.unifLightmapTexture = glGetUniformLocation(_sceneShader, "unifLightmapTexture");
  _sceneShaderParams.unifCameraTransform = glGetUniformLocation(_sceneShader, "unifCameraTransform");
  _sceneShaderParams.unifProjTransform = glGetUniformLocation(_sceneShader, "unifProjTransform");

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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, screenWidth, screenHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
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

    // Reuse the depth texture from the scene FBO!
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _sceneDepthTexture, 0);
  }

  if (hasErrors()) {
    cerr << "failed to generate buffers\n";
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

  optional<HitScanResult> result = HitScan::findFaceIndex(map, _camera.location, _camera.forward());

  // Render all the solid geometry in the map to the scene-FBO
  {
    glUseProgram(_sceneShader);
    glBindFramebuffer(GL_FRAMEBUFFER, _sceneFBO);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    glClearColor(0.6, 0.2, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update camera transform
    glm::mat4 cameraTransform = glm::lookAt(
      _camera.location, // location of camera
      _camera.location + _camera.forward(), // look at
      glm::vec3(0,0,1)  // camera up vector
    );

    glUniformMatrix4fv(_sceneShaderParams.unifCameraTransform, 1, GL_FALSE, glm::value_ptr(cameraTransform));

    // And projection transform
    glm::mat4 projectionTransform = glm::perspective(glm::radians(86.0f), 1200.0f / 800.0f, 0.5f, 10000.0f);
    glUniformMatrix4fv(_sceneShaderParams.unifProjTransform, 1, GL_FALSE, glm::value_ptr(projectionTransform));

    _renderableMap->render(_sceneShaderParams, RenderMode::SOLID, result);
  }

  // Render all the translucent geometry in the map to the effects-FBO
  {
    glUseProgram(_sceneShader);
    glBindFramebuffer(GL_FRAMEBUFFER, _effectsFBO);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT); // Don't clear the depth!

    _renderableMap->render(_sceneShaderParams, RenderMode::TRANSPARENCY, result);
  }

  // Composite them onto the screen
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  _compositingRenderer->render({_sceneTexture, _effectsTexture});

  // static int x = 0;
  // x ++;
  // if (x % 2) {
  //   _compositingRenderer.render(_sceneTexture);
  // } else {
  //   _compositingRenderer.render(_effectsTexture);
  // }
}
