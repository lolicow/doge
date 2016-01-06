
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
	vec2 currentuv;
	if (dir>0)
		currentuv.x = (1-uv.x) * frameunit + framex;
	else
		currentuv.x = uv.x * frameunit + framex;
	// next line deals with weird opengl texture uv
	//currentuv.y = uv.y * frameunit - frameunit + 1 - framey;
	//but if loaded with sdl y will be inversed.
	currentuv.y = (1-uv.y) * frameunit + framey;


	if (textureSize(Texture,0).x>1) // Is the texture defined?
		{
		vec4 texturecolor = texture(Texture, currentuv);
		gl_FragColor.xyzw = texturecolor.xyzw;

		}
	else 
		gl_FragColor.xyzw = vec4(1,0,0,1);
}
