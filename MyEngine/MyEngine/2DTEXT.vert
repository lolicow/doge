
#version 330

in vec4 coord;
out vec2 texcoord;

uniform int width;
uniform int height;

void main() {
  gl_Position = vec4(coord.x*2.0/width - 1.0,coord.y*2.0/height - 1.0,0,1);
  //gl_Position = vec4(coord.x/100.0,coord.y/100.0, 0, 1);
  texcoord.x = coord.z;
  texcoord.y = coord.w;
}