#ifndef BSP_H
#define BSP_H

#include "support.h"

namespace BSP {
  typedef struct {
    int offset;
    int length;
  } direntry_t;

  typedef struct {
    char magic[4];
    int version;
    direntry_t direntries[17];
  } header_t;

  void debugString(const BSP::header_t* header);
};

#endif