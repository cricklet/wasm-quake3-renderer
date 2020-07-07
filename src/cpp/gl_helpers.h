#ifndef GL_HELPERS_H
#define GL_HELPERS_H

#include "support.h"

bool _hasErrors(const char *filename, int line);
#define hasErrors() _hasErrors(__FILE__, __LINE__)

struct EBO {
  GLuint buffer;
};

struct VBO {
  GLuint buffer;
  size_t stride;
};

std::ostream& operator<<(std::ostream& os, const VBO& buffers);
std::ostream& operator<<(std::ostream& os, const EBO& buffers);

namespace GLHelpers {
  optional<GLuint> compileShader(const char *fileContents, int fileLength, GLenum shaderType);
  optional<GLuint> compileShaderProgram(const char* vert, int vertLength, const char* frag, int fragLength);
  optional<GLuint> loadTexture(const void* image, int width, int height);
  VBO generateRandomColorsVBO(int num);
}

#endif