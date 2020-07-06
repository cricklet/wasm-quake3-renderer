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

void ResourceManager::loadResource(const LoadResource& message) {
  MessageBindings::sendMessageToWeb(message);
  _loadingResources.insert(message.resourceID);
}

void ResourceManager::loadShaders(const LoadShaders& message) {
  MessageBindings::sendMessageToWeb(message);
  _loadingResources.insert(message.resourceID);
}


void ResourceManager::handleMessageFromWeb(const LoadedImage& message) {
  optional<GLuint> tex = GLHelpers::loadTexture(message.pointer, message.width, message.height);

  free(message.pointer);

  if (tex) {
    cout << "adding texture for " << message.resourceID << "\n";
    _textures[message.resourceID] = *tex;
    _loadingResources.erase(message.resourceID);
    return;
  }
  cerr << "failed to load texture\n";
}

void ResourceManager::handleMessageFromWeb(const LoadedBSP& message) {
  cout << "adding map for " << message.resourceID << "\n";
  _map = (const BSPMap*) message.pointer;
  _loadingResources.erase(message.resourceID);
}

void ResourceManager::handleMessageFromWeb(const LoadedShaders& message) {
  cout << "starting to compile shaders for " << message.resourceID << "\n";
  optional<GLuint> shaderProgram = GLHelpers::compileShaderProgram((const char*) message.vertPointer, (const char*) message.fragPointer);

  free(message.vertPointer);
  free(message.fragPointer);

  if (shaderProgram) {
    cout << "adding shader program for " << message.resourceID << "\n";
    _shaderPrograms[message.resourceID] = *shaderProgram;
    _loadingResources.erase(message.resourceID);
    return;
  }
  cerr << "failed to create shader program\n";
}

optional<GLuint> ResourceManager::getShaderProgram(int resourceID) {
  if (_shaderPrograms.count(resourceID)) {
    return _shaderPrograms.at(resourceID);
  }

  return {};
}

optional<GLuint> ResourceManager::getTextures(int resourceID) {
  if (_textures.count(resourceID)) {
    return _textures.at(resourceID);
  }

  return {};
}

const BSPMap* ResourceManager::getMap() {
  return _map;
}
