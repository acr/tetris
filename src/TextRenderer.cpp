#include "TextRenderer.h"
#include <iostream>
#include <utility>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace gfx {

TextRenderer::TextRenderer(GLint shader_program, const std::string& face_file_path) :
  vao(0),
  vbo(0),
  text_color_location(0) {
  FT_Library ftLibrary;
  FT_Face face;
  if(FT_Init_FreeType(&ftLibrary) != 0) {
    std::cerr << "Error initializing FreeType library" << std::endl;
    return;
  }

  if(FT_New_Face(ftLibrary, face_file_path.c_str(), 0, &face) != 0) {
    std::cerr << "Failed to load font: " << face_file_path << std::endl;
    return;
  }

  FT_Set_Pixel_Sizes(face, 0, 48);

  for(GLubyte c = 0; c < 128; c++) {
    if(FT_Load_Char(face, c, FT_LOAD_RENDER) != 0) {
      std::cerr << "Failed to load glyph " << static_cast<int>(c) <<
	" in face: " << face_file_path << std::endl;
      continue;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    dsp::checkGL();
    glBindTexture(GL_TEXTURE_2D, texture);
    dsp::checkGL();
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RED,
      face->glyph->bitmap.width,
      face->glyph->bitmap.rows,
      0,
      GL_RED,
      GL_UNSIGNED_BYTE,
      face->glyph->bitmap.buffer
      );
    dsp::checkGL();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    dsp::checkGL();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    dsp::checkGL();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    dsp::checkGL();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    dsp::checkGL();
    
    Character character = {
      texture,
      glm::ivec2(face->glyph->bitmap.width,
		 face->glyph->bitmap.rows),
      glm::ivec2(face->glyph->bitmap_left,
		 face->glyph->bitmap_top - face->glyph->bitmap.rows),
      face->glyph->advance.x
    };
    characters.insert(std::make_pair(c, character));
  }

  FT_Done_Face(face);
  FT_Done_FreeType(ftLibrary);

  glUseProgram(shader_program);
  dsp::checkGL();
  text_color_location = glGetUniformLocation(shader_program, "textColor");
  dsp::checkGL();

  glGenVertexArrays(1, &vao);
  dsp::checkGL();
  glGenBuffers(1, &vbo);
  dsp::checkGL();
  glBindVertexArray(vao);
  dsp::checkGL();
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  dsp::checkGL();
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, 0, GL_DYNAMIC_DRAW);
  dsp::checkGL();
  glEnableVertexAttribArray(0);
  dsp::checkGL();
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  dsp::checkGL();
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  dsp::checkGL();
  glBindVertexArray(0);
  dsp::checkGL();
}

TextRenderer::~TextRenderer() {
}

void TextRenderer::renderText(const std::string& text,
			       GLfloat x,
			       GLfloat y,
			       GLfloat scale,
			       const glm::vec3& color) {
  glUniform3f(text_color_location, color.r, color.g, color.b);
  dsp::checkGL();
  glActiveTexture(GL_TEXTURE0);
  dsp::checkGL();
  glBindVertexArray(vao);
  dsp::checkGL();

  for(std::string::const_iterator t = text.begin();
      t != text.end(); t++) {
    CharacterMap::const_iterator charIt = characters.find(*t);
    if(charIt == characters.end()) {
      std::cerr << "character '" << *t << "' (int: " <<
	static_cast<int>(*t) << ") not found in character map, skipping" <<
	std::endl;
      continue;
    }
    const Character& c = charIt->second;

    GLfloat xpos = x + c.bearing.x * scale;
    GLfloat ypos = y + c.bearing.y * scale;

    GLfloat w = c.size.x * scale;
    GLfloat h = c.size.y * scale;

    // Stretch the rectangular glyph texture over 2 triangles
    // Notice how the first two coordinates in each 4-element vector are
    // the vertex coordinates specified in pixel-space, while the last two
    // coordinates are the corresponding texture coordinates specified in
    // [0, 1] texture-space
    GLfloat vertices[6][4] = {
      { xpos, ypos + h, 0.0, 0.0 },
      { xpos, ypos, 0.0, 1.0 },
      { xpos + w, ypos, 1.0, 1.0 },

      { xpos, ypos + h, 0.0, 0.0 },
      { xpos + w, ypos, 1.0, 1.0 },
      { xpos + w, ypos + h, 1.0, 0.0 }
    };

    glBindTexture(GL_TEXTURE_2D, c.textureID);
    dsp::checkGL();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    dsp::checkGL();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    dsp::checkGL();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dsp::checkGL();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    dsp::checkGL();

    // Advance specified in 1/64 pixels, so convert to pixels with bitshift-
    // multiply a la '>> 6'
    x += (c.advance >> 6) * scale;
  }

  // Unbind stuff
  glBindVertexArray(0);
  dsp::checkGL();
  glBindTexture(GL_TEXTURE_2D, 0);
  dsp::checkGL();
}

}
