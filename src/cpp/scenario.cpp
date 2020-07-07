#include "scenario.h"
#include "resources.h"
#include "gl_helpers.h"
#include "bsp.h"
#include "pprint.hpp"


void TestScenario::startLoading() {
  ResourceManager::getInstance()->loadResource({
    "./data/poptart.jpg",
    ResourceType::IMAGE_FILE,
    TEXTURE_ID
  });

  ResourceManager::getInstance()->loadShaders({
    "./shader/test.vert",
    "./shader/test.frag",
    SHADER_ID
  });
}

bool TestScenario::finishLoading() {
  if (!ResourceManager::getInstance()->finishedLoading()) {
    cerr << "tried to load scenario before resources finished loading\n";
    return false;
  }

  ////////////////////////////////////////////////////////////////////////////
  // Working through open.gl again...

  // Save the following attribute configuration as a vao (vertex array object)
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  ////////////////////////////////////////////////////////////////////////////
  // Generate EBO
  GLuint ebo;
  glGenBuffers(1, &ebo);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

  ////////////////////////////////////////////////////////////////////////////
  // Generate VBO
  GLuint vbo;
  glGenBuffers(1, &vbo);

  // Choose the vbo...
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // Copy the vertex data into the vbo
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  ////////////////////////////////////////////////////////////////////////////
  // Get the test shader program
  optional<GLuint> shaderProgram = ResourceManager::getInstance()->getShaderProgram(SHADER_ID);
  if (!shaderProgram) {
    cerr << "failed to load shader program\n";
    return false;
  }

  // Use the program...
  glLinkProgram(*shaderProgram);
  glUseProgram(*shaderProgram);

  ////////////////////////////////////////////////////////////////////////////
  // Specify the inputs

  // For the vertices
  GLint posAttrib = glGetAttribLocation(*shaderProgram, "position");
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glEnableVertexAttribArray(posAttrib);

  // And colors
  GLint texAttrib = glGetAttribLocation(*shaderProgram, "texCoord");
  glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));
  glEnableVertexAttribArray(texAttrib);

  if (hasErrors()) {
    return false;
  }

  return true;
}

void TestScenario::render() {
  glClearColor(0.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  // glDrawArrays(GL_TRIANGLES, 0, 3);
  glDrawElements(GL_TRIANGLES, sizeof(elements) / sizeof(elements[0]), GL_UNSIGNED_INT, 0);

  hasErrors();
}

void BSPScenario::startLoading() {
  ResourceManager::getInstance()->loadResource({
    "./data/aerowalk.bsp",
    ResourceType::BSP_FILE,
    BSP_ID
  });

  ResourceManager::getInstance()->loadShaders({
    "./shader/bsp.vert",
    "./shader/bsp.frag",
    SHADER_ID
  });
}

bool BSPScenario::finishLoading() {
  if (!ResourceManager::getInstance()->finishedLoading()) {
    cerr << "tried to load scenario before resources finished loading\n";
    return false;
  }

  const BSPMap* map = ResourceManager::getInstance()->getMap();
  map->print();
  // map->printVertices();
  // map->printFaces();
  // map->printMeshverts();

  {
    const BSP::face_t* faces = map->faces();
    const int numFaces = map->numFaces();

    const BSP::vertex_t* vertices = map->vertices();

    _verticesPerFace.reserve(numFaces);
    _colorsPerFace.reserve(numFaces);

    for (int faceIndex = 0; faceIndex < numFaces; faceIndex ++) {
      const BSP::face_t* face = faces + faceIndex;

      VBO& verticesVBO = _verticesPerFace[faceIndex];
      VBO& colorsVBO = _colorsPerFace[faceIndex];

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
    }
  }

  // Load the shader
  GLuint shaderProgram = *ResourceManager::getInstance()->getShaderProgram(SHADER_ID);

  // Use the program...
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  // Bind the inputs
  _inPosition = glGetAttribLocation(shaderProgram, "inPosition");
  glEnableVertexAttribArray(_inPosition);

  _inColor = glGetAttribLocation(shaderProgram, "inColor");
  glEnableVertexAttribArray(_inColor);

  _unifCameraTransform = glGetUniformLocation(shaderProgram, "unifCameraTransform");
  _unifProjTransform = glGetUniformLocation(shaderProgram, "unifProjTransform");

  if (hasErrors()) {
    return false;
  }

  return true;
}

glm::vec3 Camera::forward() {
  return glm::vec3(
    sin(pitch) * cos(-yaw),
    sin(pitch) * sin(-yaw),
    -cos(pitch)
  );
}

glm::vec3 Camera::right() {
  return glm::cross(forward(), upApproximate());
}

glm::vec3 Camera::upApproximate() {
  return glm::vec3(0,0,-1);
}

void BSPScenario::think(glm::vec2 dir, double pitch, double yaw) {
  _camera.pitch = pitch;
  _camera.yaw = yaw;
  _camera.location += _camera.forward() * (dir.y * 10.0f);
  _camera.location += _camera.right() * (dir.x * 10.0f);
}

void BSPScenario::render() {
  const BSPMap* map = ResourceManager::getInstance()->getMap();

  glClearColor(0.6, 0.2, 0.6, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);

  // Update camera transform
  glm::mat4 cameraTransform = glm::lookAt(
    _camera.location, // location of camera
    _camera.location + _camera.forward(), // look at
    glm::vec3(0,0,1)  // camera up vector
  );

  glUniformMatrix4fv(_unifCameraTransform, 1, GL_FALSE, glm::value_ptr(cameraTransform));

  // And projection transform
  glm::mat4 projectionTransform = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.5f, 10000.0f);
  glUniformMatrix4fv(_unifProjTransform, 1, GL_FALSE, glm::value_ptr(projectionTransform));

  {
    const BSP::face_t* faces = map->faces();
    const int numFaces = map->numFaces();

    const BSP::meshvert_t* meshverts = map->meshverts();

    for (int faceIndex = 0; faceIndex < numFaces; faceIndex ++) {
      const BSP::face_t* face = faces + faceIndex;

      VBO& verticesVBO = _verticesPerFace[faceIndex];
      VBO& colorsVBO = _colorsPerFace[faceIndex];

      // Bind vertices
      glBindBuffer(GL_ARRAY_BUFFER, verticesVBO.buffer);
      glVertexAttribPointer(
        _inPosition, 3, GL_FLOAT, GL_FALSE,
        verticesVBO.stride /* stride */,
        (void*) 0 /* offset */);

      // Bind colors
      glBindBuffer(GL_ARRAY_BUFFER, colorsVBO.buffer);
      glVertexAttribPointer(
        _inColor, 3, GL_FLOAT, GL_FALSE,
        colorsVBO.stride /* stride */,
        (void*) 0 /* offset */);

      // Render elements
      glDrawElements(GL_TRIANGLES, face->n_meshverts, GL_UNSIGNED_INT, meshverts + face->meshvert);
    }
  }

  hasErrors();
}
