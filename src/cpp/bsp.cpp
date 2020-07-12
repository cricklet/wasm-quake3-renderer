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


void header_t::printTextures() const {
  const direntry_t* texturesEntry = this->texturesEntry();

  cout << "textures {\n";

  const int offset = texturesEntry->offset;
  const int length = texturesEntry->length;
  cout << "  offset: " << offset << " length: " << length << "\n";
  cout << "  num: " << numTextures() << "\n";

  for (int i = 0; i < 40; i ++) {
    cout << "  " << *(textures() + i) << "\n";
  }

  cout << "}\n";
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

  for (int i = 0; i < 20; i ++) {
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

std::ostream& operator<<(std::ostream& os, const BSP::texture_t& texture) {
  os << "meshvert { name:" << texture.name << " }";
  return os;
}

std::ostream& operator<<(std::ostream& os, const BSP::face_t& face) {
  os << "face { "
     << "vertex: " << face.vertex
     << ", n_vertices: " << face.n_vertices
     << ", texture: " << face.texture
     << ", meshvert: " << face.meshvert
     << ", n_meshverts " << face.n_meshverts
     << ", lm_index: " << face.lm_index
     << ", lm_start" << face.lm_start
     << ", lm_size: " << face.lm_size
     << " }";
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
