#ifndef RESOURCES_H
#define RESOURCES_H

#include "support.h"
#include "bindings.h"
#include "texture.h"

namespace BSP {
  struct header_t;
};

using BSPMap = BSP::header_t;

struct RenderableTextureOptions;
struct ResourceManager;

enum class HasResourcesState {
  NOT_STARTED,
  STILL_REQUESTING,
  ALL_RESOURCES_REQUESTED,
  FAILED
};

struct IHasResources {
  IHasResources();
  virtual ~IHasResources();

private:
  friend ResourceManager;
  virtual void load() = 0;
  virtual HasResourcesState loadingState() const = 0;
};

enum class LoadingState {
  LOADING,
  DONE,
  FAILED
};

struct ResourceManager : IMessageHandler {
public:
  static shared_ptr<ResourceManager> getInstance();
  static int nextID();

  LoadingState think();

  void addResourceLoader(IHasResources* loader);
  void removeResourceLoader(IHasResources* loader);

  bool hasOutstandingResources() const;

  void loadResource(const LoadResource& message);
  void loadShaders(const LoadShaders& message);

  void handleMessageFromWeb(const LoadedTexture& message);
  void handleMessageFromWeb(const MissingTexture& message);
  void handleMessageFromWeb(const LoadedBSP& message);
  void handleMessageFromWeb(const LoadedShaders& message);
  void handleMessageFromWeb(const LoadedTextureOptions& message);

  // TODO add a way to clear the resources after use

  optional<GLuint> getShaderProgram(int resourceID);
  optional<GLuint> getTexture(int resourceID);
  optional<RenderableTextureOptions> getTextureOptions(int resourceID);
  ResourcePtr<const BSPMap> getMap();

private:
  unordered_set<int> _loadingResources = {};
  unordered_set<int> _failedResources = {};

  unordered_map<int, GLuint> _shaderPrograms = {};
  unordered_map<int, GLuint> _textures = {};

  // Also called "texture shaders" -- these are loaded from the `data/scripts` directory
  // and include information about how to render individual textures.
  unordered_map<int, RenderableTextureOptions> _textureOptions = {};

  ResourcePtr<const BSPMap> _map = nullptr;

  unordered_set<IHasResources*> _resourceLoaders;

  static shared_ptr<ResourceManager> _instance;
};

#endif