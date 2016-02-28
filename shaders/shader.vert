#version 330

in vec2 vertex_position;
in vec3 vertex_color;
out vec4 interp_color;

uniform mat4 proj_mat;
uniform mat4 view_mat;
uniform mat4 model_mat;

void main() {
  gl_Position = proj_mat * view_mat * model_mat * vec4(vertex_position, 0.0, 1.0);
  interp_color = vec4(vertex_color, 1.0);
}
