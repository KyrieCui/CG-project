#version 330 core
layout (location = 0) in vec3 vertex; // <vec2 position, vec2 texCoords>
layout (location = 1) in vec2 texcoord; // <vec2 position, vec2 texCoords>
layout (location = 3) in vec3 normal;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



out vec2 TexCoords;
out vec3 oNormal;
out vec3 oFragPos;

void main()
{
    oNormal = mat3(transpose(inverse(model))) * normalize(normal);//���շ���
	oFragPos = vec3(model * vec4(vertex.xyz, 1.0));//��������λ��
    TexCoords = texcoord;//��������
    gl_Position = projection *view* model * vec4(vertex.xyz, 1.0);
}