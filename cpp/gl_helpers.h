#ifndef GL_HELPERS_H
#define GL_HELPERS_H

#include "support.h"

bool _hasErrors(const char *filename, int line);
#define hasErrors() _hasErrors(__FILE__, __LINE__)

namespace GLHelpers {
  optional<GLuint> compileShader(const char *fileContents, GLenum shaderType);
  optional<GLuint> compileShaderProgram(const char* vert, const char* frag);
  optional<GLuint> loadTexture(const void* image, int width, int height);
}

#endif