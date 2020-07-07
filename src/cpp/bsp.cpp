#include "bsp.h"
#include "pprint.hpp"

using namespace BSP;

void header_t::print() const {
  printf("map {\n");
  printf(" magic: %s\n", this->magic);
  printf(" version: %d\n", this->version);
  for (const direntry_t& entry : this->direntries) {
    printf(" entry offset: %d, size: %d\n", entry.offset, entry.length);
  }
  printf("}\n");
}

void header_t::printVertices() const {
  const direntry_t* verticesEntry = this->verticesEntry();

  cout << "vertices {\n";

  const int offset = verticesEntry->offset;
  const int length = verticesEntry->length;
  cout << "  offset: " << offset << " length: " << length << "\n";
  cout << "  num: " << numVertices() << "\n";

  for (int i = 0; i < 5; i ++) {
    cout << "  " << *(vertices() + i) << "\n";
  }

  cout << "}\n";
}


void header_t::printFaces() const {
  const direntry_t* facesEntry = this->facesEntry();

  cout << "faces {\n";

  const int offset = facesEntry->offset;
  const int length = facesEntry->length;
  cout << "  offset: " << offset << " length: " << length << "\n";
  cout << "  num: " << numFaces() << "\n";

  for (int i = 0; i < 5; i ++) {
    cout << "  " << *(faces() + i) << "\n";
  }

  cout << "}\n";
}


void header_t::printMeshverts() const {
  const direntry_t* meshvertsEntry = this->meshvertsEntry();

  cout << "meshverts {\n";

  const int offset = meshvertsEntry->offset;
  const int length = meshvertsEntry->length;
  cout << "  offset: " << offset << " length: " << length << "\n";
  cout << "  num: " << numMeshverts() << "\n";

  for (int i = 0; i < 5; i ++) {
    cout << "  " << *(meshverts() + i) << "\n";
  }

  cout << "}\n";
}

std::ostream& operator<<(std::ostream& os, const BSP::face_t& face) {
  os << "face { meshvert:" << face.meshvert << ", n_meshverts" << face.n_meshverts << " }";
  return os;
}

std::ostream& operator<<(std::ostream& os, const BSP::vertex_t& vertex) {
  os << "vertex { position:" << vertex.position << " }";
  return os;
}

std::ostream& operator<<(std::ostream& os, const BSP::meshvert_t& meshvert) {
  os << "meshvert { offset:" << meshvert.offset << " }";
  return os;
}
