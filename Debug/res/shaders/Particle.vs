#version 330 core
layout (location = 0) in vec3 vertex; // <vec2 position, vec2 texCoords>
layout (location = 1) in vec2 texcoord; // <vec2 position, vec2 texCoords>
out vec2 TexCoords;
uniform vec3 offset;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = texcoord;
	
    gl_Position = projection *view* model * vec4(vertex.xyz + offset.xyz, 1.0);
	gl_PointSize = 10.0;
	
}