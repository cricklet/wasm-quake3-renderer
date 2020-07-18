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

bool RenderableBSP::finishLoading() {
  const BSPMap* map = _map.get();
  if (!map) {
    cerr << "map failed to load\n";
    return false;
  }

  map->print();
  map->printTextures();
  // map->printVertices();
  map->printFaces();
  // map->printMeshverts();

  { // Load lightmaps
    const BSP::lightmap_t* lightmaps = map->lightmaps();
    const int numLightmaps = map->numLightmaps();
    for (int i = 0; i < numLightmaps; i ++) {
      const BSP::lightmap_t* lightmap = lightmaps + i;
      optional<GLuint> textureId = GLHelpers::loadTexture(lightmap, 128, 128, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
      if (textureId) {
        _lightmapTextures[i] = *textureId;
      } else {
        cerr << "failed to load lightmap " << i << "\n";
        return false;
      }
    }

    unsigned char white[] = { 255, 255, 255, 255 };
    optional<GLuint> textureId = GLHelpers::loadTexture(
        &white[0], 1, 1, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
    if (textureId) {
      _fallbackLightmapTexture = *textureId;
    } else {
      cerr << "failed to create fallback lightmap\n";
      return false;
    }
  }

  {
    const BSP::face_t* faces = map->faces();
    const int numFaces = map->numFaces();

    const BSP::vertex_t* vertices = map->vertices();
    const BSP::meshvert_t* meshverts = map->meshverts();

    _verticesPerFace.reserve(numFaces);
    _colorsPerFace.reserve(numFaces);
    _elementsPerFace.reserve(numFaces);

    for (int faceIndex = 0; faceIndex < numFaces; faceIndex ++) {
      const BSP::face_t* face = faces + faceIndex;
      if (face->type == 2) {
        continue;
      }

      VBO& verticesVBO = _verticesPerFace[faceIndex];
      VBO& colorsVBO = _colorsPerFace[faceIndex];
      EBO& elementsEBO = _elementsPerFace[faceIndex];

      // Load vertices
      glGenBuffers(1, &(verticesVBO.buffer));
      glBindBuffer(GL_ARRAY_BUFFER, verticesVBO.buffer);
      glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(BSP::vertex_t) * face->n_vertices,
        &((vertices + face->vertex)->position),
        GL_STATIC_DRAW);

      verticesVBO.stride = sizeof(BSP::vertex_t);

      // Load colors
      colorsVBO = GLHelpers::generateRandomColorsVBO(face->n_vertices);

      // Load EBO
      glGenBuffers(1, &(elementsEBO.buffer));
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsEBO.buffer);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * face->n_meshverts,
        meshverts + face->meshvert,
        GL_STATIC_DRAW);
    }
  }

  return true;
}

void RenderableBSP::render(const ShaderParameters& inputs) {
  const BSPMap* map = _map.get();
  if (!map) {
    cerr << "map failed to load\n";
    return;
  }

  const BSP::face_t* faces = map->faces();
  const int numFaces = map->numFaces();

  const BSP::texture_t* textures = map->textures();
  const int numTextures = map->numTextures();

  const BSP::meshvert_t* meshverts = map->meshverts();

  for (int faceIndex = 0; faceIndex < numFaces; faceIndex ++) {
    const BSP::face_t* face = faces + faceIndex;
    if (face->type == 2) {
      // Only render mesh faces
      continue;
    }

    VBO& verticesVBO = _verticesPerFace[faceIndex];
    VBO& colorsVBO = _colorsPerFace[faceIndex];
    EBO& elementsEBO = _elementsPerFace[faceIndex];

    // Bind the texture
    int textureOffset = face->texture;
    if (textureOffset >= 0 && textureOffset < numTextures) {
      const BSP::texture_t* texture = textures + textureOffset;
      const int textureResourceId = _textureResourceIds[string(texture->name)];
      optional<GLuint> textureId = ResourceManager::getInstance()->getTexture(textureResourceId);
      if (textureId) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, *textureId);
        glUniform1i(inputs.unifTexture, 0);
      }
    }

    // Bind the lightmap
    int lightmapIndex = face->lm_index;
    if (const GLuint* textureId = getValue(_lightmapTextures, lightmapIndex)) {
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, *textureId);
      glUniform1i(inputs.unifLightmapTexture, 1);
    } else {
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, _fallbackLightmapTexture);
      glUniform1i(inputs.unifLightmapTexture, 1);
    }

    // Bind vertices
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO.buffer);
    glVertexAttribPointer(
      inputs.inPosition, 3, GL_FLOAT, GL_FALSE,
      verticesVBO.stride /* stride */,
      (void*) 0 /* offset */);

    // Bind texture coordinates
    glVertexAttribPointer(
      inputs.inTextureCoords, 2, GL_FLOAT, GL_FALSE,
      verticesVBO.stride /* stride */,
      (void*) offsetof(BSP::vertex_t, texcoord) /* offset */);

    // Bind lightmap coordinates
    glVertexAttribPointer(
      inputs.inLightmapCoords, 2, GL_FLOAT, GL_FALSE,
      verticesVBO.stride /* stride */,
      (void*) offsetof(BSP::vertex_t, lmcoord) /* offset */);

    // Bind colors
    glBindBuffer(GL_ARRAY_BUFFER, colorsVBO.buffer);
    glVertexAttribPointer(
      inputs.inColor, 3, GL_FLOAT, GL_FALSE,
      colorsVBO.stride /* stride */,
      (void*) 0 /* offset */);

    // Render elements
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsEBO.buffer);
    glDrawElements(GL_TRIANGLES, face->n_meshverts, GL_UNSIGNED_INT, 0);
  }
}