#include "bsp.h"

void BSP::debugString(const BSP::header_t* header) {
  printf("map {\n");
  printf(" magic: %s\n", header->magic);
  printf(" version: %d\n", header->version);
  for (const direntry_t& entry : header->direntries) {
    printf(" entry offset: %d, size: %d\n", entry.offset, entry.length);
  }
  printf("}\n");
}