
#version 330

layout(location = 0)in vec3 vertexPosition;
layout(location = 1)in vec3 vertexColor;
layout(location = 2)in vec2 vertexuv;

uniform mat3 transform;
uniform mat4 perspective;
uniform vec2 cameratransform;
uniform float z;
uniform float direction;
uniform float width;
uniform float height;

out float dir;
out vec2 uv;
out vec3 color;

void main()
{      
	
	vec3 vertex2d = vec3(vertexPosition.x,vertexPosition.y,1.0);
	vec3 worldVertex = vec3(transform * vertex2d);
	vec4 worldcoord =  vec4(worldVertex,1.0);
	worldcoord.x = worldcoord.x - cameratransform.x;
	worldcoord.y = worldcoord.y - cameratransform.y;
	worldcoord.z = z;
	
	dir = direction;
	uv = vertexuv;
	color = vertexColor;

    gl_Position =  perspective * worldcoord;
}
