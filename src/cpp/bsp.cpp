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

  const auto printVertex = [](const vertex_t* v) {
    cout << "    position: " << v->position << "\n";
    cout << "    texcoord: " << v->texcoord << "\n";
    cout << "    normal: " << v->normal << "\n";
    cout << "    color: " << v->color << "\n";
  };

  cout << "vertices {\n";

  const int offset = verticesEntry->offset;
  const int length = verticesEntry->length;
  cout << "  offset: " << offset << " length: " << length << "\n";

  cout << "  num: " << numVertices() << "\n";

  {
    cout << "  first vertex: {\n";
    const vertex_t* vertex = vertices();
    printVertex(vertex);
    cout << "  }\n";
  }

  {
    cout << "  last vertex: {\n";
    const vertex_t* vertex = vertices() + numVertices() - 1;
    printVertex(vertex);
    cout << "  }\n";
  }

  cout << "}\n";
}


void header_t::printFaces() const {
  const direntry_t* facesEntry = this->facesEntry();

  const auto printFace = [](const face_t* f) {
    cout << "    texture: " << f->texture << "\n";
    cout << "    effect: " << f->effect << "\n";
    cout << "    type: " << f->type << "\n";
    cout << "    vertex: " << f->vertex << "\n";
    cout << "    n_vertices: " << f->n_vertices << "\n";
    cout << "    meshvert: " << f->meshvert << "\n";
    cout << "    n_meshverts: " << f->n_meshverts << "\n";
    cout << "    lm_index: " << f->lm_index << "\n";
    cout << "    lm_start: " << f->lm_start << "\n";
    cout << "    lm_size: " << f->lm_size << "\n";
    cout << "    lm_origin: " << f->lm_origin << "\n";
    cout << "    lm_vecs: " << f->lm_vecs << "\n";
    cout << "    normal: " << f->normal << "\n";
    cout << "    size: " << f->size << "\n";
  };

  cout << "faces {\n";

  const int offset = facesEntry->offset;
  const int length = facesEntry->length;
  cout << "  offset: " << offset << " length: " << length << "\n";

  cout << "  num: " << numFaces() << "\n";

  {
    cout << "  first face: {\n";
    const face_t* face = faces();
    printFace(face);
    cout << "  }\n";
  }

  {
    cout << "  last face: {\n";
    const face_t* face = faces() + numFaces() - 1;
    printFace(face);
    cout << "  }\n";
  }

  cout << "}\n";
}


void header_t::printMeshverts() const {
  const direntry_t* meshvertsEntry = this->meshvertsEntry();

  const auto printMeshvert = [](const meshvert_t* mv) {
    cout << "    offset: " << mv->offset << "\n";
  };

  cout << "meshverts {\n";

  const int offset = meshvertsEntry->offset;
  const int length = meshvertsEntry->length;
  cout << "  offset: " << offset << " length: " << length << "\n";

  cout << "  num: " << numMeshverts() << "\n";

  {
    cout << "  first meshvert: {\n";
    const meshvert_t* meshvert = meshverts();
    printMeshvert(meshvert);
    cout << "  }\n";
  }

  {
    cout << "  last meshvert: {\n";
    const meshvert_t* meshvert = meshverts() + numMeshverts() - 1;
    printMeshvert(meshvert);
    cout << "  }\n";
  }

  cout << "}\n";
}
