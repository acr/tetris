#include "gl_utils.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <sstream>

using namespace std;

namespace dsp {
  void printShaderInfoLog(GLuint shader_index) {
    char log[2048];
    int actualLength = 0;
    glGetShaderInfoLog(shader_index, sizeof(log), &actualLength, log);
    checkGL();
    std::cerr << "Shader info log for GL index " << shader_index << std::endl <<
      log << std::endl;
  }

  GLuint compileShader(GLuint shaderType, const char** source) {
    GLuint cs = glCreateShader(shaderType);
    checkGL();
    glShaderSource(cs, 1, source, 0);
    checkGL();
    glCompileShader(cs);
    checkGL();

    int params = -1;
    glGetShaderiv(cs, GL_COMPILE_STATUS, &params);
    checkGL();
    if(params != GL_TRUE) {
      std::cerr << "ERROR: GL shader index " << cs << " did not compile" << std::endl;
      printShaderInfoLog(cs);
      return 0;
    }
    return cs;
  }

  GLuint compileShader(GLuint shaderType, const char* source) {
    const char* s = source;
    return compileShader(shaderType, &s);
  }

  void printProgramInfoLog(GLuint program) {
    char log[2048];
    int actualLength = 0;
    glGetProgramInfoLog(program, sizeof(log), &actualLength, log);
    checkGL();
    std::cerr << "Program info log for GL index " << program << std::endl <<
      log << std::endl;
  }

  const char* GLTypeToString(GLenum type) {
    switch(type) {
    case GL_BOOL: return "bool";
    case GL_INT: return "int";
    case GL_FLOAT: return "float";
    case GL_FLOAT_VEC2: return "vec2";
    case GL_FLOAT_VEC3: return "vec3";
    case GL_FLOAT_VEC4: return "vec4";
    case GL_FLOAT_MAT2: return "mat2";
    case GL_FLOAT_MAT3: return "mat3";
    case GL_FLOAT_MAT4: return "mat4";
    case GL_SAMPLER_2D: return "sampler2D";
    case GL_SAMPLER_3D: return "sampler3D";
    case GL_SAMPLER_CUBE: return "samplerCube";
    case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
    default: break;
    }
    return "other";
  }

  int checkGlProgramLink(GLuint program) {
    int params = -1;
    glGetProgramiv(program, GL_LINK_STATUS, &params);
    checkGL();
    if(params != GL_TRUE) {
      std::cerr << "ERROR: could not link shader program with GL index " << program << std::endl;
      printProgramInfoLog(program);
      return 0;
    }
    return 1;
  }

  void printAll(GLuint program) {
    std::cout << "*************** shader program " << program << " info ***************" << std::endl;
    int params = -1;
    glGetProgramiv(program, GL_LINK_STATUS, &params);
    checkGL();
    std::cout << "GL_LINK_STATUS: " << params << std::endl;
    glGetProgramiv(program, GL_ATTACHED_SHADERS, &params);
    checkGL();
    std::cout << "GL_ATTACHED_SHADERS: " << params << std::endl;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &params);
    checkGL();
    std::cout << "GL_ACTIVE_ATTRIBUTES: " << params << std::endl;

    int location = 0;
    char name[64];
    char longName[64];
    int actualLength = 0;
    int size = 0;
    GLenum type;
    for(int i = 0; i < params; ++i) {
      glGetActiveAttrib(program, i, sizeof(name), &actualLength,
			&size, &type, name);
      checkGL();
      if(size > 1) {
	for(int j = 0; j < size; ++j) {
	  sprintf(longName, "%s[%i]", name, j);
	  location = glGetAttribLocation(program, longName);
	  checkGL();
	  std::cout << " " << i <<
	    ") type: " << GLTypeToString(type) <<
	    " name: " << longName <<
	    " location: " << location << std::endl;
	}
      } else {
	location = glGetAttribLocation(program, name);
	checkGL();
	std::cout << " " << i <<
	  ") type: " << GLTypeToString(type) <<
	  " name: " << name <<
	  " location: " << location << std::endl;
      }
    }
  
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &params);
    checkGL();
    std::cout << "GL_ACTIVE_UNIFORMS: " << params << std::endl;
    for(int i = 0; i < params; ++i) {
      glGetActiveUniform(program, i, sizeof(name), &actualLength,
			 &size, &type, name);
      checkGL();
      if(size > 1) {
	for(int j = 0; j < size; ++j) {
	  sprintf(longName, "%s[%i]", name, j);
	  location = glGetUniformLocation(program, longName);
	  checkGL();
	  std::cout << " " << i <<
	    ") type: " << GLTypeToString(type) <<
	    " name: " << longName <<
	    " location: " << location << std::endl;
	}
      } else {
	location = glGetUniformLocation(program, name);
	checkGL();
	std::cout << " " << i <<
	  ") type: " << GLTypeToString(type) <<
	  " name: " << name <<
	  " location: " << location << std::endl;
      }
    }

    GLint blocks = 0;
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &blocks);
    checkGL();
    std::cout << "GL_MAX_UNIFORM_BUFFER_BINDINGS = " << blocks << std::endl;

    printProgramInfoLog(program);
  }

  int readFile(const char* filePath, char** dst) {
    ifstream fd(filePath, std::ios::binary);
    if(!fd.is_open()) {
      std::cerr << "ERROR opening file " << filePath << std::endl;
      return -1;
    }

    fd.seekg(0, ios_base::end);
    const size_t fileLength = fd.tellg();
    fd.seekg(0, ios_base::beg);

    char* d = new char[fileLength+1];
    fd.read(d, fileLength);
    d[fileLength] = '\0';
    if(static_cast<size_t>(fd.tellg()) != fileLength) {
      std::cerr << "ERROR, read " << static_cast<int>(fd.tellg()) <<
	" bytes, but not equal to " << static_cast<int>(fileLength) <<
	" bytes expected" << std::endl;
      fd.close();
      return -1;
    }
    fd.close();
    *dst = d;
    return fileLength;
  }

  void printGLInfo() {
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    checkGL();
    std::cout << "OpenGL version supported: " << glGetString(GL_VERSION) << std::endl;
    checkGL();
  }

  void checkGL() {
    std::string glError;
    switch(glGetError()) {
    case GL_NO_ERROR: {
      break;
    }

    case GL_INVALID_ENUM: {
      glError = "GL_INVALID_ENUM";
      break;
    }

    case GL_INVALID_VALUE: {
      glError = "GL_INVALID_VALUE";
      break;
    }

    case GL_INVALID_OPERATION: {
      glError = "GL_INVALID_OPERATION";
      break;
    }

    case GL_INVALID_FRAMEBUFFER_OPERATION: {
      glError = "GL_INVALID_FRAMEBUFFER_OPERATION";
      break;
    }

    case GL_OUT_OF_MEMORY: {
      glError = "GL_OUT_OF_MEMORY";
      break;
    }

    case GL_STACK_UNDERFLOW: {
      glError = "GL_STACK_UNDERFLOW";
      break;
    }

    case GL_STACK_OVERFLOW: {
      glError = "GL_STACK_OVERFLOW";
      break;
    }
    }

    if(glError.size() > 0) {
      std::stringstream errMsg;
      errMsg << "OpenGL error: " << glError;
      std::cerr << errMsg.str() << std::endl;
      throw std::runtime_error(errMsg.str().c_str());
    }
  }

  bool compile_shaders(const char* vertex_shader_path, const char* fragment_shader_path,
		       GLint& shader_program, PreLinkCallback* cb) {
    char* vertex_shader_source = 0;
    if(readFile(vertex_shader_path, &vertex_shader_source) == -1) {
      return false;
    }
    char* fragment_shader_source = 0;
    if(readFile(fragment_shader_path, &fragment_shader_source) == -1) {
      return false;
    }
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertex_shader_source);
    if(vs == 0) {
      return false;
    }
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragment_shader_source);
    if(fs == 0) {
      return false;
    }

    delete [] vertex_shader_source;
    delete [] fragment_shader_source;

    shader_program = glCreateProgram();
    glAttachShader(shader_program, fs);
    checkGL();
    glAttachShader(shader_program, vs);
    checkGL();

    cb->preLinkCallback(shader_program);

    glLinkProgram(shader_program);
    checkGL();
    if(checkGlProgramLink(shader_program) == 0) {
      return false;
    }

    return true;
  }
}
