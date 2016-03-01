#ifndef GL_UTILS_H
#define GL_UTILS_H

#include "gl_defns.h"

namespace dsp {

  void printShaderInfoLog(GLuint shader_index);
  GLuint compileShader(GLuint shaderType, const char** source);
  GLuint compileShader(GLuint shaderType, const char* source);
  void printProgramInfoLog(GLuint program);
  const char* GLTypeToString(GLenum type);
  int checkGlProgramLink(GLuint program);
  void printAll(GLuint program);
  int readFile(const char* filePath, char** dst);
  void printGLInfo();
  void checkGL();

  class PreLinkCallback {
  public:
    virtual ~PreLinkCallback() {}
    virtual void preLinkCallback(GLint shader_program)=0;
  };

  bool compile_shaders(const char* vertex_shader_path, const char* fragment_shader_path,
		       GLint& shader_program, PreLinkCallback* cb);
}

#endif
