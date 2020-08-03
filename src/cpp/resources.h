#ifndef RESOURCES_H
#define RESOURCES_H

#include "support.h"
#include "texture.h"

namespace BSP {
  struct header_t;
};

using BSPMap = BSP::header_t;

struct RenderableTextureOptions;
struct ResourceManager;

struct IHasResources {
  IHasResources();
  virtual ~IHasResources();

private:
  friend ResourceManager;
  virtual bool finishLoading() = 0;
};

enum class LoadingState {
  LOADING,
  DONE,
  FAILED
};

#endif