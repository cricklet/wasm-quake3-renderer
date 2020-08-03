#include "resources.h"
#include "resource_manager.h"
#include "gl_helpers.h"
#include "bsp.h"

IHasResources::IHasResources() {
  ResourceManager::getInstance()->addResourceLoader(this);
}

IHasResources::~IHasResources() {
  ResourceManager::getInstance()->removeResourceLoader(this);
}

shared_ptr<ResourceManager> ResourceManager::_instance = nullptr;

shared_ptr<ResourceManager> ResourceManager::getInstance() {
  if (!_instance) {
    _instance = make_shared<ResourceManager>();
  }

  return _instance;
}

int ResourceManager::nextID() {
  static int resourceID = 0;
  return resourceID ++;
}

void ResourceManager::addResourceLoader(IHasResources* loader) {
  _resourceLoaders[loader] = HasResourcesFinished::NO;
}

void ResourceManager::removeResourceLoader(IHasResources* loader) {
  _resourceLoaders.erase(loader);
}

bool ResourceManager::hasOutstandingResources() const {
  return _loadingResources.size() > 0 || _failedResources.size() > 0;
}

LoadingState ResourceManager::think() {
  unordered_set<IHasResources*> incompleteLoaders;
  for (const auto& it : _loadingResources) {
    incompleteLoaders.insert(it.second);
  }

  for (auto& loaderAndResult : _resourceLoaders) {
    if (incompleteLoaders.count(loaderAndResult.first)) {
      // Still loading... let's wait
      assert(loaderAndResult.second != HasResourcesFinished::YES);
      continue;
    }
    if (loaderAndResult.second != HasResourcesFinished::NO) {
      // Done -- either failed or finished
      continue;
    }

    bool success = loaderAndResult.first->finishLoading();
    loaderAndResult.second = success ? HasResourcesFinished::YES : HasResourcesFinished::FAILED;
  }

  for (auto& it : _resourceLoaders) {
    if (it.second == HasResourcesFinished::NO) {
      // At least one loader is still loading
      return LoadingState::LOADING;
    }
    if (it.second == HasResourcesFinished::FAILED) {
      // At least one loader failed
      return LoadingState::FAILED;
    }
  }

  if (hasOutstandingResources()) {
    return LoadingState::LOADING;
  }

  if (_failedResources.size()) {
    return LoadingState::FAILED;
  }

  return LoadingState::DONE;
}


void ResourceManager::loadResource(IHasResources* loader, const LoadResource& message) {
  MessageBindings::sendMessageToWeb(message);
  _loadingResources[message.resourceID] = loader;
}

void ResourceManager::loadShaders(IHasResources* loader, const LoadShaders& message) {
  MessageBindings::sendMessageToWeb(message);
  _loadingResources[message.resourceID] = loader;
}


void ResourceManager::handleMessageFromWeb(const LoadedTexture& message) {
  optional<GLuint> tex = GLHelpers::loadTexture(message.pointer, message.width, message.height);
  free(message.pointer);

  if (tex) {
    cout << "adding texture for " << message.resourceID << "\n";
    _textures[message.resourceID] = *tex;
    _loadingResources.erase(message.resourceID);
    return;
  }

  cerr << "error while loading texture\n";
  _failedResources.insert(message.resourceID);
}

void ResourceManager::handleMessageFromWeb(const MissingTexture& message) {
    cout << "missing texture (not error) for " << message.resourceID << "\n";
  _loadingResources.erase(message.resourceID);
}

void ResourceManager::handleMessageFromWeb(const LoadedBSP& message) {
  cout << "adding map for " << message.resourceID << "\n";
  _map = (const BSPMap*) message.pointer;
  _loadingResources.erase(message.resourceID);
}

void ResourceManager::handleMessageFromWeb(const LoadedShaders& message) {
  cout << "starting to compile shaders for " << message.resourceID << "\n";
  optional<GLuint> shaderProgram = GLHelpers::compileShaderProgram((const char*) message.vertPointer, message.vertLength, (const char*) message.fragPointer, message.fragLength);

  free(message.vertPointer);
  free(message.fragPointer);

  if (shaderProgram) {
    cout << "adding shader program for " << message.resourceID << "\n";
    _shaderPrograms[message.resourceID] = *shaderProgram;
    _loadingResources.erase(message.resourceID);
    return;
  }
  cerr << "failed to create shader program\n";
  _failedResources.insert(message.resourceID);
}

void ResourceManager::handleMessageFromWeb(const LoadedTextureOptions& message) {
  _textureOptions[message.resourceID] = { message.surfaceParamTrans };
}

optional<GLuint> ResourceManager::getShaderProgram(int resourceID) {
  if (_shaderPrograms.count(resourceID)) {
    return _shaderPrograms.at(resourceID);
  }

  return {};
}

optional<GLuint> ResourceManager::getTexture(int resourceID) {
  if (_textures.count(resourceID)) {
    return _textures.at(resourceID);
  }

  return {};
}

optional<RenderableTextureOptions> ResourceManager::getTextureOptions(int resourceID) {
  if (_textureOptions.count(resourceID)) {
    return _textureOptions.at(resourceID);
  }

  return {};
}

ResourcePtr<const BSPMap> ResourceManager::getMap() {
  return _map;
}
