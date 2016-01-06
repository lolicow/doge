
#version 330
in vec2 texcoord;
uniform sampler2D tex;
uniform vec3 color;
uniform float blendindex;

void main(void) {
	gl_FragColor = vec4(color.r,color.g,color.b, texture2D(tex, texcoord).r * blendindex);
}