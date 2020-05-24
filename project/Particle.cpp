#include "Particle.h"



ParticleGenerator::ParticleGenerator(Shader _shader, Texture2D _texture2d, GLuint _amount, int _model)
{
	m_shader = _shader;
	m_texture2d = _texture2d;
	m_amount = _amount;
	m_model = _model;
	this->Init();
}


ParticleGenerator::~ParticleGenerator()
{
}


void ParticleGenerator::Init()
{
	//��ʼ��VA0
	{
		GLuint VBO;
		GLfloat particle_point[] = {
			0.0f, 0.0f, 0.0f, 0.0f,0.0f
		};
		glGenVertexArrays(1, &this->m_VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(this->m_VAO);
		// Fill mesh buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(particle_point), particle_point, GL_STATIC_DRAW);
		// Set mesh attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glBindVertexArray(0);
	}

	//��ʼ����������
	for (int i = 0; i < this->m_amount; i++)
	{
		this->m_particles.push_back(Particle());
		GeneretorParticle(this->m_particles[i]);
	}
}
//
void  ParticleGenerator::GeneretorParticle(Particle &_particle)
{
	if (m_model == 0)//ѩ��
	{
		//����ֵ
		_particle.Life = (rand() % 10);
		//λ��
		_particle.Position.x = ((rand() % 1000) - 500);
		_particle.Position.y = 500 + ((rand() % 100) - 50);
		_particle.Position.z = 500 + ((rand() % 1000) - 500);
		//��ɫ
		_particle.Color = glm::vec3(1.0, 1.0, 1.0);
		//�ٶ�
		_particle.Velocity.x = (rand() % 10) - 5.0;
		_particle.Velocity.z = (rand() % 10) - 5.0;
		_particle.Velocity.y = -(rand() % 10);
	}
	
	if (m_model == 1)//��Ȫ
	{
		//����ֵ
		_particle.Life = (rand() % 10)-8;
		//λ��
		_particle.Position.x = 0;
		_particle.Position.y = 0;
		_particle.Position.z = 0;
		//��ɫ
		_particle.Color = glm::vec3(1.0, 1.0, 1.0);
		//�ٶ�
		float ang = (rand() % 360);
		_particle.Velocity.x =3 * sin(ang /360 * 3.1415925*2);
		_particle.Velocity.z =3 * cos(ang / 360 * 3.1415925*2);
		_particle.Velocity.y = 10;
		//���ٶ�
		_particle.Accel.x = 0;
		_particle.Accel.y = -0.2;
		_particle.Accel.z = -0.0;
	}


		
}
void ParticleGenerator::Draw(glm::mat4 modelview)
{
	Update(0.1);
	//�����㾫��ģʽ ���õ������
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_PROGRAM_POINT_SIZE);
	this->m_shader.Use();
	this->m_shader.SetMatrix4("model", modelview);
	for (int i = 0; i < this->m_amount; i++)
	{
		this->m_shader.SetVector3f("offset", this->m_particles[i].Position);
		this->m_shader.SetVector3f("startColor", this->m_particles[i].Color);
		glActiveTexture(GL_TEXTURE0);
		this->m_texture2d.Bind();
		glBindVertexArray(this->m_VAO);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
	}
	glUseProgram(0);
	//�رյ㾫��ģʽ ���õ������
	glDisable(GL_POINT_SPRITE);
}
void ParticleGenerator::Update(float dt)
{
	
	if (m_model == 0)
	{
		for (int i = 0; i < this->m_amount; i++)
		{

			//---1 ����
		   //����λ��
			this->m_particles[i].Position.x += this->m_particles[i].Velocity.x * dt;
			this->m_particles[i].Position.y += this->m_particles[i].Velocity.y * dt;
			this->m_particles[i].Position.z += this->m_particles[i].Velocity.z * dt;
			//��������
			this->m_particles[i].Life -= dt * 0.01;


			////---2 ���
			////�����ľ�
			if (this->m_particles[i].Life < 0)
				GeneretorParticle(this->m_particles[i]);
			////��ʱ �߽���
			if (this->m_particles[i].Position.y < 100 && this->m_particles[i].Position.x<180 && this->m_particles[i].Position.x > -150)
				GeneretorParticle(this->m_particles[i]);
			if (this->m_particles[i].Position.y < 0)
				GeneretorParticle(this->m_particles[i]);
		}
	}

	if (m_model == 1)
	{
		for (int i = 0; i < this->m_amount; i++)
		{

			//---1 ����
		   //����λ��
			this->m_particles[i].Position.x += this->m_particles[i].Velocity.x * dt;
			this->m_particles[i].Position.y += this->m_particles[i].Velocity.y * dt;
			this->m_particles[i].Position.z += this->m_particles[i].Velocity.z * dt;

			//�����ٶ�
			this->m_particles[i].Velocity.x += this->m_particles[i].Accel.x;
			this->m_particles[i].Velocity.y += this->m_particles[i].Accel.y;
			this->m_particles[i].Velocity.z += this->m_particles[i].Accel.z;
			//��������
			this->m_particles[i].Life -= dt * 0.01;


			////---2 ���
			////�����ľ�
			if (this->m_particles[i].Life < 0)
				GeneretorParticle(this->m_particles[i]);
			////��ʱ �߽���
			if (this->m_particles[i].Position.y < -1)
				GeneretorParticle(this->m_particles[i]);
			
		}
	}

}