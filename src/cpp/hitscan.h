#ifndef HITSCAN_H
#define HITSCAN_H

#include "support.h"

namespace BSP {
  struct header_t;
  struct face_t;
  struct meshvert_t;
}
using BSPMap = BSP::header_t;

struct HitScanResult {
  const BSP::face_t* face;
  const BSP::meshvert_t* mesh;
  float distance;
};

namespace HitScan {
  optional<HitScanResult> findFaceIndex(const BSPMap* map, glm::vec3 location, glm::vec3 direction);
}

#endif