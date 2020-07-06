#ifndef SCENARIO_H
#define SCENARIO_H

#include "support.h"

struct IScenario {
public:
  virtual ~IScenario() {}
  virtual void startLoading();
  virtual bool finishLoading();
  virtual void think();
  virtual void render();
};

struct TestScenario : IScenario {
public:
  void startLoading() override;
  bool finishLoading() override;
  void think() override;
  void render() override;

private:
  static const int SHADER_ID = 1;
  static const int TEXTURE_ID = 2;

  static constexpr float vertices[] = {
    // Position   Texcoords
    -0.5f,  0.5f, 0.0f, 0.0f, // Top-left
    0.5f,  0.5f, 1.0f, 0.0f, // Top-right
    0.5f, -0.5f, 1.0f, 1.0f, // Bottom-right
    -0.5f, -0.5f, 0.0f, 1.0f  // Bottom-left
  };

  static constexpr GLuint elements[] = {
    0, 1, 2,
    2, 3, 0
  };
};

struct BSPScenario : IScenario {
public:
  void startLoading() override;
  bool finishLoading() override;
  void think() override;
  void render() override;

private:
  static const int BSP_ID = 0;
  static const int SHADER_ID = 1;

  static constexpr float testVertices[] = {
    // front
    -1.0, -1.0,  1.0,
     1.0, -1.0,  1.0,
     1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,
    // back
    -1.0, -1.0, -1.0,
     1.0, -1.0, -1.0,
     1.0,  1.0, -1.0,
    -1.0,  1.0, -1.0
  };

  static constexpr GLuint testMeshverts[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
  };

  static const int TEST_FACE_ID = -1;

  struct FaceBuffers {
    GLuint vertexBuffer;
    GLuint elementsBuffer;
    GLuint colorsBuffer;
  };

  // VBO holding vertices for each face (from face.vertex & .n_vertices)
  unordered_map<int, FaceBuffers> _faceVBOs;

  // When rendering:
  // > glBindBuffer(GL_ARRAY_BUFFER, _faceVBOs[idx].vertexBuffer);
  // > glVertexAttribPointer(vertexPosAttr, 3, GL_FLOAT, GL_FALSE, sizeof(BSP::vertex_t), (void*)(0));
  // > glDrawElements(GL_TRIANGLES, face.n_meshverts, GL_UNSIGNED_INT, map->meshVertices() + face.meshvert);
  
  GLuint _inPosition;
  GLuint _inColor;
  GLuint _unifCameraTransform;
  GLuint _unifProjTransform;
};

#endif