#version 330

in vec4 interp_color;
out vec4 frag_color;

void main() {
  frag_color = interp_color;
}
