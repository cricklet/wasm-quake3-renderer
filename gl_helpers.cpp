#include "gl_helpers.h"

bool _hasErrors(const char *filename, int line) {
  bool errored = false;

  GLenum error;
  while ((error = glGetError()) != GL_NO_ERROR) {
    string str;

    switch (error) {
      case GL_INVALID_OPERATION:
        str = "INVALID_OPERATION";
        break;
      case GL_INVALID_ENUM:
        str = "INVALID_ENUM";
        break;
      case GL_INVALID_VALUE:
        str = "INVALID_VALUE";
        break;
      case GL_OUT_OF_MEMORY:
        str = "OUT_OF_MEMORY";
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        str = "INVALID_FRAMEBUFFER_OPERATION";
        break;
    }

    // cerr << "GL_" << str.c_str() << "\n";
    cerr << "GL_" << str.c_str() << " - " << filename << ":" << line << "\n";
    errored = true;
  }

  return errored;
}

optional<GLuint> compileShader(const char *fileContents, GLenum shaderType) {
  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &fileContents, NULL);
  glCompileShader(shader);

  // Print shader compile errors.
  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    char buffer[512];
    glGetShaderInfoLog(shader, 512, NULL, buffer);
    cerr << "Error buffer: \n" << buffer << "\n\n";
    cerr << "Failed to compile:\n" << fileContents << "\n\n";

    return {};
  }

  return shader;
}

optional<GLuint> compileShaderProgram(const char* vert, const char* frag) {
  optional<GLuint> vertShader = compileShader(vert, GL_VERTEX_SHADER);
  optional<GLuint> fragShader = compileShader(frag, GL_FRAGMENT_SHADER);

  if (!vertShader) {
    return {};
  }

  if (!fragShader) {
    return {};
  }

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, *vertShader);
  glAttachShader(shaderProgram, *fragShader);
  if (hasErrors()) {
    return {};
  }

  glLinkProgram(shaderProgram);
  if (hasErrors()) {
    return {};
  }

  return shaderProgram;
}