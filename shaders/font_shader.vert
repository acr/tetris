#version 330

layout (location = 0) in vec4 vertex;
out vec2 TexCoords;

uniform mat4 proj_mat;
uniform mat4 view_mat;

void main() {
  gl_Position = proj_mat * view_mat * vec4(vertex.xy, 0.0, 1.0);
  TexCoords = vertex.zw;
}
