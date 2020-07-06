#ifndef RESOURCES_H
#define RESOURCES_H

#include "support.h"
#include "bindings.h"

namespace BSP {
  struct header_t;
};

using BSPMap = BSP::header_t;

struct ResourceManager : IMessageHandler {
public:
  static shared_ptr<ResourceManager> getInstance();

  void loadResource(const LoadResource& message);
  void loadShaders(const LoadShaders& message);

  void handleMessageFromWeb(const LoadedImage& message);
  void handleMessageFromWeb(const LoadedBSP& message);
  void handleMessageFromWeb(const LoadedShaders& message);

  // TODO add a way to clear the resources after use

  optional<GLuint> getShaderProgram(int resourceID);
  optional<GLuint> getTextures(int resourceID);
  const BSPMap* getMap();

private:
  unordered_set<int> _loadingResources = {};

  unordered_map<int, GLuint> _shaderPrograms = {};
  unordered_map<int, GLuint> _textures = {};

  const BSPMap* _map = nullptr;

  static shared_ptr<ResourceManager> _instance;
};

#endif