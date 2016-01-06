
#version 330
uniform sampler2D Texture;
uniform float frameunit;
uniform float framex;
uniform float framey;

in float dir;
in vec2 uv;
in vec3 color;


void main()
{
		gl_FragColor = vec4(1,0,0,1);

}
