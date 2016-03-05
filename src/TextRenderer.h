#ifndef TEXT_RENDERER
#define TEXT_RENDERER

#include "gl_defns.h"
#include "gl_utils.h"
#include <map>
#include <string>

namespace gfx {

class TextRenderer {
private:

  /**
   * A container for OpenGL and FreeType data regarding a glyph.
   * size: The width (x) and height (y) of the glyph in pixels
   * bearing: The glyph's horizontal (x) and vertical (y)
   *          offset from the origin
   * advance: The horizontal distance between this glyph's origin
   *          and the next glyph's origin, specified in increments
   *          of 1/64 pixels
   */
  struct Character {
    GLuint textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    long int advance;
  };

  typedef std::map<GLchar, Character> CharacterMap;
  CharacterMap characters;
  GLuint vao;
  GLuint vbo;
  GLint text_color_location;

public:
  TextRenderer(GLint shader_program, const std::string& face_file_path);
  ~TextRenderer();

  void renderText(const std::string& text, GLfloat x,
		  GLfloat y, GLfloat scale,
		  const glm::vec3& color);
};

}

#endif
