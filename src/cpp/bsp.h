#ifndef BSP_H
#define BSP_H

#include "support.h"

namespace BSP {
  struct direntry_t {
    int offset;
    int length;
  };

  struct vertex_t {
    float position[3];
    float texcoord[2][2]; //	Vertex texture coordinates. 0=surface, 1=lightmap.
    float normal[3]; //	Vertex normal.
    char color[4]; //	Vertex color. RGBA.
  };

  struct face_t {
    int texture; //	Texture index.
    int effect; //	Index into lump 12 (Effects), or -1.
    int type; //	Face type. 1=polygon, 2=patch, 3=mesh, 4=billboard
    int vertex; //	Index of first vertex.
    int n_vertices; //	Number of vertices.
    int meshvert; //	Index of first meshvert.
    int n_meshverts; //	Number of meshverts.
    int lm_index; //	Lightmap index.
    int lm_start[2]; //	Corner of this face's lightmap image in lightmap.
    int lm_size[2]; //	Size of this face's lightmap image in lightmap.
    float lm_origin[3]; //	World space origin of lightmap.
    float lm_vecs[2][3]; //	World space lightmap s and t unit vectors.
    float normal[3]; //	Surface normal.
    int size[2]; //	Patch dimensions.
  };

  struct header_t {
    char magic[4];
    int version;
    direntry_t direntries[17];

    // Surface descriptions (assume these have been converted to OpenGL textures).
    const direntry_t* textures() const { return direntries + 1; }

    // Planes	Planes used by map geometry.
    const direntry_t* planes() const { return direntries + 2; }

    // Nodes	BSP tree nodes.
    const direntry_t* nodes() const { return direntries + 3; }

    // Leaves	BSP tree leaves.
    const direntry_t* leaves() const { return direntries + 4; }

    // Leaffaces	Lists of face indices, one list per leaf.
    const direntry_t* leaffaces() const { return direntries + 5; }

    // Models	Descriptions of rigid world geometry in map (we only use model[0]).
    const direntry_t* models() const { return direntries + 7; }

    // Vertices	Vertices used to describe faces.
    const direntry_t* vertices() const { return direntries + 10; }
    int numVertices() const { return vertices()->length / sizeof(vertex_t); }
    const vertex_t* getVertex(int i) const { return (const vertex_t*) ((char*) this + vertices()->offset + sizeof(vertex_t) * i); }

    // Meshverts	Lists of offsets, one list per mesh.
    const direntry_t* meshverts() const { return direntries + 11; }

    // Faces	Surface geometry.
    const direntry_t* faces() const { return direntries + 13; }

    // Lightmaps	Packed lightmap data (assume these have been converted to an OpenGL texture)
    const direntry_t* lightmaps() const { return direntries + 14; }

    // Visdata	Cluster-cluster visibility data.
    const direntry_t* visdata() const { return direntries + 16; }

    void print() const;
    void printVertices() const;
  };
};

using BSPMap = BSP::header_t;

#endif