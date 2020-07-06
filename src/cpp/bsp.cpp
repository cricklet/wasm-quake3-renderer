#include "bsp.h"

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
  const direntry_t* verticesEntry = this->vertices();

  const auto printVertex = [](const vertex_t* v) {
    cout << "  position: " << v->position[0] << ", " << v->position[1] << ", " << v->position[2] << "\n";
    cout << "  texcoord: " << v->texcoord[0][0] << ", " << v->texcoord[0][1] << "\n";
    cout << "            " << v->texcoord[1][0] << ", " << v->texcoord[1][1] << "\n";
    cout << "  normal:   " << v->normal[0] << ", " << v->normal[1] << ", " << v->normal[1] << "\n";
    cout << "  color:    " << (int) v->color[0] << ", " << (int) v->color[1] << ", " << (int) v->color[2] << ", " << (int) v->color[3] << "\n";
  };

  cout << "vertices {\n";

  const int offset = verticesEntry->offset;
  const int length = verticesEntry->length;
  cout << "  offset: " << offset << " length: " << length << "\n";

  cout << "  num: " << numVertices() << "\n";

  {
    const vertex_t* vertex = getVertex(0);
    printVertex(vertex);
  }

  {
    const vertex_t* vertex = getVertex(numVertices() - 1);
    printVertex(vertex);
  }

  cout << "}\n";
}
