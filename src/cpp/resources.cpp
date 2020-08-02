#include "resources.h"
#include "gl_helpers.h"
#include "bsp.h"

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

bool ResourceManager::finishedLoading() const {
  return _loadingResources.size() == 0 && _failedResources.size() == 0;
}

void ResourceManager::loadResource(const LoadResource& message) {
  MessageBindings::sendMessageToWeb(message);
  _loadingResources.insert(message.resourceID);
}

void ResourceManager::loadShaders(const LoadShaders& message) {
  MessageBindings::sendMessageToWeb(message);
  _loadingResources.insert(message.resourceID);
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
