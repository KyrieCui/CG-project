#pragma once
#include"glew.h"
#include"glm/glm.hpp"
#include"Texture2D.h"
#include"Shader.h"
#include<vector>

struct Particle
{
	glm::vec3 Position, Velocity,Accel;
	glm::vec3 Color;
	GLfloat Life;
	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};


class ParticleGenerator
{
public:
	ParticleGenerator(Shader _shader,Texture2D _texture2d,GLuint _amount,int _model = 0);//���ص�shader id�� ����ID,��������
	~ParticleGenerator();
	void Update(float dt);//dt��ʱ����
	void Draw(glm::mat4 modelview);
private:
	std::vector<Particle> m_particles;//���Ӿ�����Ϣ
	Shader     m_shader;//����shader
	Texture2D  m_texture2d;//��������
	GLuint     m_amount;//������Ŀ
	void      Init();//��ʼ������
	void      GeneretorParticle(Particle &_particle);//��������
	GLuint    m_VAO;
	int     m_model;//0 -ѩ��    1-��Ȫ
};

