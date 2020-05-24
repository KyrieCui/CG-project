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
	ParticleGenerator(Shader _shader,Texture2D _texture2d,GLuint _amount,int _model = 0);//加载的shader id， 纹理ID,粒子总数
	~ParticleGenerator();
	void Update(float dt);//dt：时间间隔
	void Draw(glm::mat4 modelview);
private:
	std::vector<Particle> m_particles;//粒子具体信息
	Shader     m_shader;//粒子shader
	Texture2D  m_texture2d;//粒子纹理
	GLuint     m_amount;//粒子数目
	void      Init();//初始化粒子
	void      GeneretorParticle(Particle &_particle);//出生粒子
	GLuint    m_VAO;
	int     m_model;//0 -雪花    1-喷泉
};

