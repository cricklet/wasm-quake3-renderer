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

optional<GLuint> GLHelpers::compileShader(const char *fileContents, int fileLength, GLenum shaderType) {
  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &fileContents, &fileLength);
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

optional<GLuint> GLHelpers::compileShaderProgram(const char* vert, int vertLength, const char* frag, int fragLength) {
  optional<GLuint> vertShader = compileShader(vert, vertLength, GL_VERTEX_SHADER);
  optional<GLuint> fragShader = compileShader(frag, fragLength, GL_FRAGMENT_SHADER);

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

optional<GLuint> GLHelpers::loadTexture(const void* image, int width, int height) {
  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                GL_UNSIGNED_BYTE, image);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glGenerateMipmap(GL_TEXTURE_2D);

  cout << "loaded texture with dimensions " << width << ", " << height << " into " << tex << "\n";

  if (hasErrors()) {
    cout << "failed to load texture\n";
    return {};
  }

  return tex;
}

VBO GLHelpers::generateRandomColorsVBO(int num) {
  float values[num * 3];
  for (int i = 0; i < num * 3; i ++) {
    values[i] = 0.5 + 0.5 * float(rand())/float(RAND_MAX);
  }

  VBO result;

  glGenBuffers(1, &(result.buffer));
  glBindBuffer(GL_ARRAY_BUFFER, result.buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(values), values, GL_STATIC_DRAW);

  result.stride = sizeof(float) * 3;

  return result;
}

std::ostream& operator<<(std::ostream& os, const VBO& buffers) {
  os << "{" << buffers.buffer << ", " << buffers.stride << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, const EBO& buffers) {
  os << "{" << buffers.buffer << "}";
  return os;
}
