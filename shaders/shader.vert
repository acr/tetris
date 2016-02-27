#version 330

in vec3 vertex_position;
in vec3 vertex_color;
out vec4 interp_color;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

void main() {
  gl_Position = P * V * M * vec4(vertex_position, 1.0);
  interp_color = vec4(vertex_color, 1.0);
}
