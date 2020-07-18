#include "renderable.h"

#include "bsp.h"
#include "gl_helpers.h"
#include "resources.h"

bool RenderableBSP::loadDependencies() {
  static bool loadingSecondaryResources = false;
  if (loadingSecondaryResources) {
    return false;
  }

  const BSPMap* map = _map.get();
  if (!map) {
    cerr << "map failed to load\n";
    return false;
  }

  const BSP::texture_t* textures = map->textures();
  const int numTextures = map->numTextures();

  int textureResourceId = 100;

  for (int textureIndex = 0; textureIndex < numTextures; textureIndex ++) {
    const BSP::texture_t* texture = textures + textureIndex;
    
    _textureResourceIds[string(texture->name)] = textureResourceId;
    ResourceManager::getInstance()->loadResource({
      string("./data/") + string(texture->name),
      ResourceType::IMAGE_FILE,
      textureResourceId
    });

    textureResourceId ++;
  }

  loadingSecondaryResources = true;
  return true;
}