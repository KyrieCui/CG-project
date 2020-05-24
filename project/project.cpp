// project.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

//


#include <iostream>
#include "glew.h"
#include "glut.h"//opengl头文件
#include "Graphic.h"
#include "ResourceManager.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Camera.h"
#include "Particle.h"
#pragma comment(lib,"glut32.lib")//opengl库
#pragma comment(lib,"glew32.lib")//opengl库
#pragma comment(lib,"soil2-debug.lib")


GLfloat m_tranlate[3]    = {0,0,-10};//用于平移，对应X Y Z 平移量。按键W:上    S:下    A:左  D:右
GLfloat m_rorate[2]      = {0,0}  ;//用于旋转，分别绕X轴和Y轴旋转的角度，用鼠标左键控制
GLfloat m_scale          = 1.0    ;//用于缩放，用鼠标中间滚轮控制
GLint   m_MouseDownPT[2] = {0,0}  ;//记录鼠标坐标点，用于控制旋转角度
bool    m_bMouseDown     = false  ;//记录鼠标左键是否按下，按下为TRUE,初始值为false

float fogtex = 0.0;//让TV雾的纹理坐标变动
const GLuint WIDTH = 1200, HEIGHT = 800;
GraphicRenderer  *Renderer;//图形
ParticleGenerator * m_ParticleSnow, *m_ParticleFountain;//粒子系统
Camera camera(glm::vec3(0.0f, 45.0f, 460.0f), glm::vec3(0.0f, 25.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

float m_robot[3] = {0,-200,-150};//0-绕X轴旋转的角度  1-X位置  2--Y位置
float m_robotPos[5][2] = { {-200,-150},{-100,-90},{30,-70},{30,150},{0,-50} };//robot经过的位置，每个位置停留一会
float m_robotSleepTime = 100;//robot 停留时间，默认0开始运动，到达位置后赋值100  自己减--
float m_robotJoint = 0;//robot控制关节的角度
float m_robotStpe = 5;//关节旋转角度
int   m_posID = 1;//当前位置号；

void init()
{
	{
		glEnable(GL_DEPTH_TEST);
	}
	//加载资源
	//1 加载shader文件 
	{
		//shader 1
		ResourceManager::LoadShader("res/shaders/Graphic.vs", "res/shaders/Graphic.frag", nullptr, "Graphic");
		glm::mat4 projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 1000.0f);
		ResourceManager::GetShader("Graphic").Use().SetInteger("image", 0);
		ResourceManager::GetShader("Graphic").Use().SetInteger("image1", 1);
		ResourceManager::GetShader("Graphic").Use().SetInteger("image2", 2);
		ResourceManager::GetShader("Graphic").SetMatrix4("projection", projection);
		glUseProgram(0);

		//shader 2
		ResourceManager::LoadShader("res/shaders/Particle.vs", "res/shaders/Particle.frag", nullptr, "Particle");
		ResourceManager::GetShader("Particle").Use().SetInteger("image", 0);
		ResourceManager::GetShader("Particle").SetMatrix4("projection", projection);
		glUseProgram(0);
		//shader 3
		ResourceManager::LoadShader("res/shaders/GraphicLight.vs", "res/shaders/GraphicLight.frag", nullptr, "GraphicLight");
		ResourceManager::GetShader("GraphicLight").Use().SetInteger("image", 0);
		ResourceManager::GetShader("GraphicLight").SetMatrix4("projection", projection);
		glUseProgram(0);

		//shader 3
		ResourceManager::LoadShader("res/shaders/GraphicTex.vs", "res/shaders/GraphicTex.frag", nullptr, "GraphicTex");
		ResourceManager::GetShader("GraphicTex").Use().SetInteger("image", 0);
		ResourceManager::GetShader("GraphicTex").Use().SetInteger("image1", 1);
		ResourceManager::GetShader("GraphicTex").Use().SetInteger("image2", 2);
		ResourceManager::GetShader("GraphicTex").SetMatrix4("projection", projection);
		glUseProgram(0);

	}
	//2 加载纹理图
	{
		ResourceManager::LoadTexture("res/textures/door.bmp", GL_FALSE, "door");
		ResourceManager::LoadTexture("res/textures/sofa.bmp", GL_FALSE, "sofa");
		ResourceManager::LoadTexture("res/textures/table.bmp", GL_FALSE, "table");
		ResourceManager::LoadTexture("res/textures/TV.bmp", GL_TRUE, "TV");
		ResourceManager::LoadTexture("res/textures/wall.bmp", GL_FALSE, "wall");
		ResourceManager::LoadTexture("res/textures/wall1.bmp", GL_FALSE, "wall1");
		ResourceManager::LoadTexture("res/textures/wood1.bmp", GL_FALSE, "wood1");
		ResourceManager::LoadTexture("res/textures/wood2.bmp", GL_FALSE, "wood2");
		ResourceManager::LoadTexture("res/textures/wood3.bmp", GL_FALSE, "wood3");
		ResourceManager::LoadTexture("res/textures/computer.bmp", GL_TRUE, "computer");
		ResourceManager::LoadTexture("res/textures/wordMap.png", GL_TRUE, "wordMap");
		ResourceManager::LoadTexture("res/textures/closet.png", GL_TRUE, "closet");
		ResourceManager::LoadTexture("res/textures/stone1.png", GL_TRUE, "stone1");
		ResourceManager::LoadTexture("res/textures/stone2.png", GL_TRUE, "stone2");
		ResourceManager::LoadTexture("res/textures/stone3.png", GL_TRUE, "stone3");
		ResourceManager::LoadTexture("res/textures/stone4.png", GL_TRUE, "stone4");
		ResourceManager::LoadTexture("res/textures/dun.png", GL_TRUE, "dun");
		ResourceManager::LoadTexture("res/textures/sound.png", GL_TRUE, "sound");
		ResourceManager::LoadTexture("res/textures/cupboard2.png", GL_TRUE, "cupboard2");
		ResourceManager::LoadTexture("res/textures/floor.png", GL_TRUE, "floor");
		ResourceManager::LoadTexture("res/textures/floor1.png", GL_TRUE, "floor1");
		ResourceManager::LoadTexture("res/textures/floor3.png", GL_TRUE, "floor3");
		ResourceManager::LoadTexture("res/textures/love.png", GL_TRUE, "love");
		ResourceManager::LoadTexture("res/textures/snow.bmp", GL_TRUE, "snow");
		ResourceManager::LoadTexture("res/textures/smoke.bmp", GL_TRUE, "smoke");
		ResourceManager::LoadTexture("res/textures/black.png", GL_TRUE, "black");
		ResourceManager::LoadTexture("res/textures/white.png", GL_TRUE, "white");
		ResourceManager::LoadTexture("res/textures/mihuang.png", GL_TRUE, "mihuang");
		ResourceManager::LoadTexture("res/textures/fog.bmp", GL_FALSE, "fog");
		ResourceManager::LoadTexture("res/textures/mountain.bmp", GL_FALSE, "mountain");
		ResourceManager::LoadTexture("res/textures/mountain2.png", GL_FALSE, "mountain2");
		ResourceManager::LoadTexture("res/textures/neptune.bmp", GL_FALSE, "neptune");
		ResourceManager::LoadTexture("res/textures/saturn.bmp", GL_FALSE, "saturn");
		ResourceManager::LoadTexture("res/textures/universe2.bmp", GL_FALSE, "universe2");
		ResourceManager::LoadTexture("res/textures/uranus.bmp", GL_FALSE, "uranus");
		ResourceManager::LoadTexture("res/textures/venus.bmp", GL_FALSE, "venus");
	}
	//3 模型数据 vbo  vao
	//

	//5 初始化对象
	{
		//Renderer = new GraphicRenderer((ResourceManager::GetShader("Graphic")));
		m_ParticleSnow = new ParticleGenerator((ResourceManager::GetShader("Particle")), ResourceManager::GetTexture("snow"), 500);
		m_ParticleFountain = new ParticleGenerator((ResourceManager::GetShader("Particle")), ResourceManager::GetTexture("snow"), 500,1);
		Renderer = new GraphicRenderer((ResourceManager::GetShader("Graphic")));

	}
}
//绘制墙壁
void  DrawWall(glm::mat4 model)
{
	//绘制地面
	//Renderer->DrawSprite(0, model, ResourceManager::GetTexture("wall1"), glm::vec3(0, -0.2, 0), glm::vec3(1000, 1000, 0.0), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//500*500
	//墙壁
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("wall"), glm::vec3(0, 50, 0), glm::vec3(300, 100, 300.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//地板 floor
	Renderer->DrawSprite(0, model, ResourceManager::GetTexture("floor"), glm::vec3(0, 99.5, 0), glm::vec3(300, 300, 0.0), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//天花板
	Renderer->DrawSprite(0, model, ResourceManager::GetTexture("floor1"), glm::vec3(0, 0.2, 0), glm::vec3(300, 300, 0.0), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
//绘制TV

void DrawTv(glm::mat4 model)
{
	//
	//Renderer->DrawSprite(0, model, ResourceManager::GetTexture("floor3"), glm::vec3(250, 50.0, -15.0), glm::vec3(1, 100, 270.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//tv
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("mountain"), glm::vec3(249, 50, -80.0), glm::vec3(2, 70, 200.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(fogtex, 1.0f, 0.0f), &ResourceManager::GetTexture("fog"),&ResourceManager::GetTexture("mountain2"));

	fogtex += 0.01;
	if (fogtex > 1.0)fogtex = 0.0;
}
//桌子
void  drawDesk(glm::mat4 model)
{
	//1 桌子面
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("wood2"), glm::vec3(0, 10, 0), glm::vec3(10, 1, 10.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//桌子腿
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("wood1"), glm::vec3(4, 5, 4), glm::vec3(1, 10, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("wood1"), glm::vec3(-4, 5, 4), glm::vec3(1, 10, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("wood1"), glm::vec3(4, 5, -4), glm::vec3(1, 10, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("wood1"), glm::vec3(-4, 5, -4), glm::vec3(1, 10, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
void  drawDesk1(glm::mat4 model)
{
	//1 桌子面
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("stone2"), glm::vec3(0, 10, 0), glm::vec3(10, 1, 10.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//桌子腿
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("wood1"), glm::vec3(4, 5, 4), glm::vec3(1, 10, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("wood1"), glm::vec3(-4, 5, 4), glm::vec3(1, 10, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("wood1"), glm::vec3(4, 5, -4), glm::vec3(1, 10, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("wood1"), glm::vec3(-4, 5, -4), glm::vec3(1, 10, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
void drawSofa(glm::mat4 model)
{
	//沙发底
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("stone1"), glm::vec3(0, 2, 0), glm::vec3(60, 4, 15.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//沙发两侧
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("stone3"), glm::vec3(28.5, 6, 0), glm::vec3(4, 4, 15.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//沙发两侧
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("stone3"), glm::vec3(-28.5, 6, 0), glm::vec3(4, 4, 15.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//沙发靠背
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("sofa"), glm::vec3(0, 11.5, 7), glm::vec3(60, 15, 4.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
void chair(glm::mat4 model)
{
	//凳子面 5*5*1
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("wood2"), glm::vec3(0, 8, 0), glm::vec3(10, 1, 10.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//桌子腿
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("stone1"), glm::vec3(4, 4, 4), glm::vec3(1, 8, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("stone1"), glm::vec3(-4, 4, 4), glm::vec3(1, 8, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("stone1"), glm::vec3(4, 4, -4), glm::vec3(1, 8, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("stone1"), glm::vec3(-4, 4, -4), glm::vec3(1, 8, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//靠背
	//Renderer->DrawSprite(1, model, ResourceManager::GetTexture("stone3"), glm::vec3(0, 13, 4.5), glm::vec3(10, 10, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
//床
void DrewBed(glm::mat4 model)
{
	//床底
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("wood1"), glm::vec3(0, 2, 0), glm::vec3(25, 4, 60.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//床垫子
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("sofa"), glm::vec3(0, 5, 0), glm::vec3(25, 2, 60.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//床靠背
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("wood2"), glm::vec3(0, 8.5, 29), glm::vec3(25, 15, 2.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

}
//衣柜
void DrawCloset(glm::mat4 model)
{
	//主体
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("stone2"), glm::vec3(0, 30, 0), glm::vec3(44, 60, 25.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//前面贴纸
	Renderer->DrawSprite(0, model, ResourceManager::GetTexture("closet"), glm::vec3(0, 30, 13.55), glm::vec3(44, 60, 1.5), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
void DrawCupboard(glm::mat4 model)
{
	//主体
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("wood1"), glm::vec3(0, 10, 0), glm::vec3(116, 20, 15.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//前面贴纸
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("closet"), glm::vec3(0, 10, 8), glm::vec3(116, 20, 1.5), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

}
//d蹲便器
void DrawToilet(glm::mat4 model)
{
	//底座
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("stone1"), glm::vec3(0, 5, 0), glm::vec3(58, 10, 58.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//床垫子
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("dun"), glm::vec3(0, 10.2, 0), glm::vec3(20, 1, 20.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


}
void DrawSound(glm::mat4 model)
{
	//主体
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("wood1"), glm::vec3(0, 17.5, 0), glm::vec3(15, 35, 20.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//前面贴纸
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("sound"), glm::vec3(-8.5, 17.5, 0), glm::vec3(2.0, 35, 20.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
void DrawRoom1(glm::mat4 model)
{
	//{
	//	//墙壁
	//	glm::mat4  model1 = glm::translate(model, glm::vec3(60.0, 0.1, 0.0));
	//	model1 = glm::scale(model, glm::vec3(1.19, 0.99, 0.99));
	//	DrawWall(model1);
	//
	//	//门
	//	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("door"), glm::vec3(160, 35, 148), glm::vec3(30, 70, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//}
	//绘制墙壁1
	{
		glm::mat4  model1 = glm::translate(model, glm::vec3(60.0, 0.1, 0.0));
		Renderer->DrawSprite(1, model, ResourceManager::GetTexture("wall1"), glm::vec3(0, 50, -250), glm::vec3(500, 100, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		Renderer->DrawSprite(1, model, ResourceManager::GetTexture("wall1"), glm::vec3(250, 50, -100), glm::vec3(1, 100, 300.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		Renderer->DrawSprite(1, model, ResourceManager::GetTexture("wall1"), glm::vec3(-250, 50, -100), glm::vec3(1, 100, 300.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		Renderer->DrawSprite(1, model, ResourceManager::GetTexture("floor"), glm::vec3(0, 100, -100), glm::vec3(500, 1, 300.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		Renderer->DrawSprite(1, model, ResourceManager::GetTexture("floor1"), glm::vec3(0, 0, -100), glm::vec3(500, 1, 300.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//	model1 = glm::scale(model, glm::vec3(1.19, 0.99, 0.99));
		//	DrawWall(model1);
	}
	//tv
	{
		//glm::mat4  model1 = glm::translate(model, glm::vec3(30, 0, -148));
	//	DrawTv(model);
	}
	//1 桌子
	{
		glm::mat4  model1 = glm::scale(model, glm::vec3(1.0, 1.0, 1.0));//桌子在x,y,z方向上分别拉伸了3,1,4倍
		//chair(model1);
		//drawDesk(model1);
	}

	//2 沙发
	{

		glm::mat4 model1 = glm::translate(model, glm::vec3(53.3, 0.0,- 80));
		model1 = glm::rotate(model1, -90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model1 = glm::scale(model1, glm::vec3(1.5, 1.2, 1.2));
		drawSofa(model1);
	}

	//音响1
	{
		glm::mat4  model1 = glm::translate(model, glm::vec3(250-16, 0.0, 30));
		DrawSound(model1);
	}
	//音响2
	{
		glm::mat4  model1 = glm::translate(model, glm::vec3(250-16, 0.0, -190.0));
		DrawSound(model1);
	}
	//橱柜1
	{
		glm::mat4  model1 = glm::translate(model, glm::vec3(184.8, 0, 8-250));
		DrawCupboard(model1);
	}
	//橱柜2
	{
		glm::mat4  model1 = glm::translate(model, glm::vec3(184.8, 78, 8-250));
		DrawCupboard(model1);
	}

	//桌子
	{
		//桌子
		{
			glm::mat4  model1 = glm::translate(model, glm::vec3(103, 0.0, -80));
			model1 = glm::rotate(model1, -90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			model1 = glm::scale(model1, glm::vec3(6.0, 0.5, 3.0));
			drawDesk1(model1);
		}
	}
}
void DrawRoom2(glm::mat4 model)
{
	////门
	//{
	//	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("door"), glm::vec3(26.06, 35, 29.55), glm::vec3(30, 70, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//}
	//
	////绘制墙壁
	//{
	//
	//	glm::mat4  model1 = glm::translate(model, glm::vec3(-45, 0.0, 71.2 + 18.0));
	//	model1 = glm::scale(model1, glm::vec3(0.7, 1.0, 0.4));
	//	DrawWall(model1);
	//}
	////壁画
	//{
	//
	//	//glm::mat4  model1 = glm::translate(model, glm::vec3(pos[0], pos[1], pos[2]));
	//	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("love"), glm::vec3(59.875, 50.0, 89.08), glm::vec3(50, 35, 1.0), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//}
	//床
	{
		glm::mat4  model1 = glm::translate(model, glm::vec3(-189, 0.0f, - 150-62));
		model1 = glm::rotate(model1, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model1 = glm::scale(model1, glm::vec3(1.4, 1.0, 1.0));
		DrewBed(model1);
	}
	////壁纸
	//{
	//
	//	glm::mat4  model1 = glm::translate(model, glm::vec3(-148.0, 0.0, 88.8));
	//	model1 = glm::rotate(model1, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	//	Renderer->DrawSprite(0, model1, ResourceManager::GetTexture("wordMap"), glm::vec3(0, 50.0, -0.5), glm::vec3(60.0, 40, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//}
	//衣柜
	{
		glm::mat4  model1 = glm::translate(model, glm::vec3(-124.488, 0.0, -200-59));
		DrawCloset(model1);
	}
	//桌子
	{
		glm::mat4  model1 = glm::translate(model, glm::vec3(0, 0.0,-200));
		model1 = glm::scale(model1, glm::vec3(3.0, 2.0, 2.0));
		drawDesk(model1);
	}
	//椅子
	{
		glm::mat4  model1 = glm::translate(model, glm::vec3(-23.8, 0.0, -150-50));
		model1 = glm::scale(model1, glm::vec3(1.5f, 1.5f, 1.5f));
		chair(model1);
	}
	//椅子
	{
		glm::mat4  model1 = glm::translate(model, glm::vec3(23.8, 0.0, -150 - 50));
		model1 = glm::scale(model1, glm::vec3(1.5f, 1.5f, 1.5f));
		chair(model1);
	}
}
void DrawRoom3(glm::mat4 model)
{
	////绘制墙壁
	//{
	//	glm::mat4  model1 = glm::translate(model, glm::vec3(-119.747, 0.0, -60.4658));
	//	model1 = glm::scale(model1, glm::vec3(0.2, 1.0, 0.6));
	//	DrawWall(model1);
	//}
	////门
	//{
	//	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("door"), glm::vec3(-90, 35, 4.45), glm::vec3(30, 70, 1.0), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//}
	////马桶
	//{
	//	glm::mat4  model1 = glm::translate(model, glm::vec3(-119.377, 0.0, -121.4475));
	//	DrawToilet(model1);
	//}
}
void DrawXueren(glm::mat4 model)
{


	//身体
	Renderer->DrawSprite(2, model, ResourceManager::GetTexture("white"), glm::vec3(0, 15.0, 0), glm::vec3(10, 10, 10.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//头
	//大球
	Renderer->DrawSprite(2, model, ResourceManager::GetTexture("white"), glm::vec3(0, 28.0, 0), glm::vec3(5, 5, 5.0), glm::vec3(0.0f, m_rorate[0], 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//眼睛
	Renderer->DrawSprite(2, model, ResourceManager::GetTexture("black"), glm::vec3(0 + 2, 28.0 + 2, 0 + 3.3), glm::vec3(1, 1, 1.0), glm::vec3(0.0f, m_rorate[0], 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Renderer->DrawSprite(2, model, ResourceManager::GetTexture("black"), glm::vec3(0 - 2, 28.0 + 2, 0 + 3.3), glm::vec3(1, 1, 1.0), glm::vec3(0.0f, m_rorate[0], 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//嘴巴
	Renderer->DrawSprite(2, model, ResourceManager::GetTexture("black"), glm::vec3(0, 28.0 - 1, 0 + 4), glm::vec3(2, 1, 2.0), glm::vec3(0.0f, m_rorate[0], 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	

}
void DrawRobot(glm::mat4 model)
{
	//身体
	Renderer->DrawSprite(2, model, ResourceManager::GetTexture("venus"), glm::vec3(0, 17.0, 0), glm::vec3(5, 6,5.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//头
	//大球
	Renderer->DrawSprite(2, model, ResourceManager::GetTexture("mihuang"), glm::vec3(0, 28.0, 0), glm::vec3(5, 5,5.0), glm::vec3(0.0f, m_rorate[0], 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//眼睛
	Renderer->DrawSprite(2, model, ResourceManager::GetTexture("black"), glm::vec3(0 + 2, 28.0 + 2, 0 + 3.3), glm::vec3(1, 1, 1.0), glm::vec3(0.0f, m_rorate[0], 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Renderer->DrawSprite(2, model, ResourceManager::GetTexture("black"), glm::vec3(0 - 2, 28.0 + 2, 0 + 3.3), glm::vec3(1, 1, 1.0), glm::vec3(0.0f, m_rorate[0], 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//嘴巴
	Renderer->DrawSprite(2, model, ResourceManager::GetTexture("black"), glm::vec3(0, 28.0 - 1, 0 + 4), glm::vec3(2, 1, 2.0), glm::vec3(0.0f, m_rorate[0], 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


	
	//绘制腿1
	{
		glm::mat4  model1 = glm::translate(model, glm::vec3(-3, 13.0, 0));
		model1 = glm::rotate(model1, m_robotJoint, glm::vec3(1, 0, 0));

		//
		model1 = glm::translate(model1, glm::vec3(0, -2.5, 0));
		Renderer->DrawSprite(1, model1, ResourceManager::GetTexture("wood1"), glm::vec3(0, 0, 0), glm::vec3(1.5, 6, 1.5), glm::vec3(0.0f, 0.0, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	//绘制腿2
	{
		glm::mat4  model1 = glm::translate(model, glm::vec3(3, 13.0, 0));
		model1 = glm::rotate(model1, -m_robotJoint, glm::vec3(1, 0, 0));

		//
		model1 = glm::translate(model1, glm::vec3(0, -2.5, 0));
		Renderer->DrawSprite(1, model1, ResourceManager::GetTexture("wood1"), glm::vec3(0, 0, 0), glm::vec3(1.5, 6, 1.5), glm::vec3(0.0f, 0.0, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	//胳膊1
	{
		glm::mat4  model1 = glm::translate(model, glm::vec3(-5, 20.0, 0));
		model1 = glm::rotate(model1, -20.0f, glm::vec3(0, 0, 1));
		model1 = glm::rotate(model1, -m_robotJoint, glm::vec3(1, 0, 0));
		//
		model1 = glm::translate(model1, glm::vec3(0, -2.5, 0));
		Renderer->DrawSprite(1, model1, ResourceManager::GetTexture("wood1"), glm::vec3(0, 0, 0), glm::vec3(1.5, 6, 1.5), glm::vec3(0.0f, 0.0, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	//胳膊2
	{
		glm::mat4  model1 = glm::translate(model, glm::vec3(5, 20.0, 0));
		model1 = glm::rotate(model1, 20.0f, glm::vec3(0, 0, 1));
		model1 = glm::rotate(model1, m_robotJoint, glm::vec3(1, 0, 0));
		//
		model1 = glm::translate(model1, glm::vec3(0, -2.5, 0));
		Renderer->DrawSprite(1, model1, ResourceManager::GetTexture("wood1"), glm::vec3(0, 0, 0), glm::vec3(1.5, 6, 1.5), glm::vec3(0.0f, 0.0, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
}
void DrawRoadLight(glm::mat4 model)
{
	//路灯
	Renderer->DrawSprite(2, model, ResourceManager::GetTexture("mihuang"), glm::vec3(110, 80.0, 180), glm::vec3(3, 3, 3.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("stone2"), glm::vec3(110, 40.0, 180), glm::vec3(1, 80, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
//重新绘制
void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// 擦除背景色和深度缓存
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//背景颜色


	//shader 1 光照
	ResourceManager::GetShader("GraphicLight").Use();
	ResourceManager::GetShader("GraphicLight").SetMatrix4("view", camera.GetViewMatrix());
	ResourceManager::GetShader("GraphicLight").SetVector3f("viewPos", camera.Position);
	glm::mat4 model;
	Renderer->SetShader(ResourceManager::GetShader("GraphicLight"));
	//绘制地面
	Renderer->DrawSprite(1, model, ResourceManager::GetTexture("smoke"), glm::vec3(0, -1.0, 0), glm::vec3(1000, 1, 1000.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//绘制雪人
	{
		m_rorate[0] += 0.5;
		glm::mat4  model1 = glm::translate(model, glm::vec3(125, 0.0, 200.0));
		model1 = glm::rotate(model1, m_rorate[0], glm::vec3(0, 1, 0));
		DrawXueren(model1);
	}
	{
		glm::mat4  model1 = glm::translate(model, glm::vec3(125, 0.0, 200.0));
		//底座
		Renderer->DrawSprite(1, model1, ResourceManager::GetTexture("floor1"), glm::vec3(0, 0.0, 0), glm::vec3(50, 10, 50.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//外壳
		Renderer->DrawSprite(2, model1, ResourceManager::GetTexture("stone1"), glm::vec3(0, 20.0, 0), glm::vec3(20, 20, 20.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glDisable(GL_BLEND);

		
	}
	




	//// shader 2  室内
	ResourceManager::GetShader("Graphic").Use();
	ResourceManager::GetShader("Graphic").SetMatrix4("view", camera.GetViewMatrix());
	Renderer->SetShader(ResourceManager::GetShader("Graphic"));
	model = glm::translate(model, glm::vec3(0, 0, 0));
	////绘制路灯
	DrawRoadLight(model);
	//房间1;
	DrawRoom1(model);
	//房间2
	DrawRoom2(model);
	//房间3
	DrawRoom3(model);

	
	//绘制小人 robot
	{
		glm::mat4  model1 = glm::translate(model, glm::vec3(m_robot[1], -6.0, m_robot[2]));
		model1 = glm::rotate(model1, m_robot[0]+90, glm::vec3(0, 1, 0));
		DrawRobot(model1);
	}

	//shader 2 雪花
	ResourceManager::GetShader("Particle").Use();
	//ResourceManager::GetShader("Particle").SetMatrix4("projection", projection);
	ResourceManager::GetShader("Particle").SetMatrix4("view", camera.GetViewMatrix());
	//绘制雪花
	m_ParticleSnow->Draw(model);


	{
		glm::mat4  model1 = glm::translate(model, glm::vec3(-125, 0.0, -30.0));
		m_ParticleFountain->Draw(model1);//绘制喷泉
		Renderer->DrawSprite(1, model1, ResourceManager::GetTexture("wood1"), glm::vec3(0, 2.0, 40), glm::vec3(80, 5, 5.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));//绘制栏杆
		Renderer->DrawSprite(1, model1, ResourceManager::GetTexture("wood1"), glm::vec3(0, 2.0, -40), glm::vec3(80, 5, 5.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));//绘制栏杆
		Renderer->DrawSprite(1, model1, ResourceManager::GetTexture("wood1"), glm::vec3(40, 2.0, 0), glm::vec3(5, 5, 80.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));//绘制栏杆
		Renderer->DrawSprite(1, model1, ResourceManager::GetTexture("wood1"), glm::vec3(-40, 2.0, 0), glm::vec3(5, 5, 80.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));//绘制栏杆
	}
	

	//shader 1 光照
	ResourceManager::GetShader("GraphicTex").Use();
	ResourceManager::GetShader("GraphicTex").SetMatrix4("view", camera.GetViewMatrix());
	ResourceManager::GetShader("GraphicTex").SetVector3f("viewPos", camera.Position);
	Renderer->SetShader(ResourceManager::GetShader("GraphicTex"));
	DrawTv(model);


	glutSwapBuffers();//交互前后缓冲区
	
}

//窗口大小改变事件  
void ReshapeEvent(int width, int height)
{
	glViewport(0, 0, width, height);//视口在屏幕的大小位置 
	//glMatrixMode(GL_PROJECTION);// 投影矩阵
	//glLoadIdentity();           // 矩阵单位
	//gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 10000.0f);// 设置投影矩阵
	//glMatrixMode(GL_MODELVIEW);	// 模型矩阵
	//glLoadIdentity();//单位矩阵化
	glEnable(GL_DEPTH_TEST);//启动深度检测
}
//空闲事件处理事件
void IdleEvent()
{
	if (m_robotSleepTime < 0)//robot 到达位置，休息一会
	{
		m_robotJoint = 0;

		m_robotSleepTime += 0.1;
	}
	else//行走过程中
	{
		m_robotJoint += m_robotStpe;
		if (m_robotJoint > 30 || m_robotJoint < -30)
			m_robotStpe = -m_robotStpe;
	}
		

	//检测是否达到位置  
	if (m_robotSleepTime > 100)//走了100多步后
	{
		float dis = sqrt((m_robotPos[m_posID][0] - m_robot[1])*(m_robotPos[m_posID][0] - m_robot[1]) + (m_robotPos[m_posID][1] - m_robot[2])*(m_robotPos[m_posID][1] - m_robot[2]));

		if (dis < 1)//到达位置
		{
			m_robotSleepTime = -20;//休息一会
			m_posID += 1;
			if (m_posID > 4)m_posID = 0;
		}
	}
	 if (m_robotSleepTime >= 0)//运动中
	{
		//计算单位方向向量
		float dis = sqrt((m_robotPos[m_posID][0] - m_robot[1])*(m_robotPos[m_posID][0] - m_robot[1]) + (m_robotPos[m_posID][1] - m_robot[2])*(m_robotPos[m_posID][1] - m_robot[2]));
		float Dirx = (m_robotPos[m_posID][0] - m_robot[1]) / dis;
		float Dirz = (m_robotPos[m_posID][1] - m_robot[2]) / dis;

		//计算角度
		m_robot[0] = acos(Dirx) / 3.1419925 * 180;
		if (Dirz > 0)
			m_robot[0] = -m_robot[0];

		//计算robot位置
		m_robot[1] += Dirx * 0.5;
		m_robot[2] += Dirz * 0.5;

		m_robotSleepTime++;
	}

		
		
    glutPostRedisplay(); //刷新函数
}
//键盘事件 默认是英文输入法下的大写字母
void KeyboardEvent(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'W'://上移动
		m_tranlate[1] += 0.1;
		camera.ForwardCameraPos();
		break;
	case 'S'://下移动
		m_tranlate[1] -= 0.1;
		camera.BackCameraPos();
		break;
	case 'A'://左移动
		m_tranlate[0] -= 0.1;
		break;
	case 'D'://右移动
		m_tranlate[0] += 0.1;
		break;
	case 'Q'://右移动
		m_tranlate[1] += 0.1;
		break;
	case 'E'://右移动
		m_tranlate[1] -= 0.1;
		break;
	}
}
//鼠标事件
void MouseEvent(int button, int state, int x, int y)
{

	//1、鼠标缩放：往上滚动放大；往下滚动缩小
	if (state == GLUT_UP && button == GLUT_WHEEL_UP)//滚轮往上滚动
	{
		m_scale += 0.1;
	}

	else if (state == GLUT_UP && button == GLUT_WHEEL_DOWN)//滚轮往下滚动
	{
		m_scale -= 0.1;
		if (m_scale < 0.1) m_scale = 0.1;
	}


	//【2 鼠标左键是否按下】
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		m_bMouseDown = true;//鼠标左键按下
		m_MouseDownPT[0] = x;//记录当前X坐标
		m_MouseDownPT[1] = y;////记录当前Y坐标
	}
	else
	{
		m_bMouseDown = false;//鼠标左键弹起，结束旋转
	}
		

	/*第一个参数表明哪个鼠标键被按下或松开，这个变量可以是下面的三个值中的一个：
	GLUT_LEFT_BUTTON
	GLUT_MIDDLE_BUTTON
	GLUT_RIGHT_BUTTON
	第二个参数表明，函数被调用发生时，鼠标的状态，也就是是被按下，或松开，可能取值如下：
	GLUT_DOWN
	GLUT_UP
	第三四个参数 XY 以左上角位原点
	*/

	

}
//按下鼠标按钮移动鼠标事件
void MotionEvent(int x, int y)
{
	if (m_bMouseDown)//如果鼠标左键被按下
	{
		m_rorate[0] += y - m_MouseDownPT[1];//通过滑动鼠标改变旋转的角度
		m_rorate[1] += x - m_MouseDownPT[0];//通过滑动鼠标改变旋转的角度
		camera.ProcessMouseMovement((x - m_MouseDownPT[0])*0.01, 0);// (y - m_MouseDownPT[1])*0.01);

		m_MouseDownPT[0] = x;//记录当前X坐标
		m_MouseDownPT[1] = y;//记录当前Y坐标
	}
}
//检测鼠标进入或离开窗口
void MouseEntry(int state)
{
	/*state有两个值表明是离开还是进入窗口:
	GLUT_LEFT
	GLUT_ENTERED*/
}
//点击菜单响应事件
void MenuEvent(int choose)
{
	switch (choose)
	{
	case 1://复位：把旋转平移缩放的值复位
		//用于平移，对应X Y Z 平移量。按键W:上  S:下   A:左  D：右
		m_tranlate[0] = 0;
		m_tranlate[1] = 0;
		m_tranlate[2] = -10;

		//用于旋转，分别是绕X轴 和Y轴旋转的角度，用鼠标左键控制
		m_rorate[0] = 0;
		m_rorate[1] = 0;

		//用于缩放，用鼠标中间滚轮控制
		m_scale = 1.0;

		//记录鼠标坐标点，用于控制旋转角度；
		m_MouseDownPT[0] = 0;
		m_MouseDownPT[1] = 0;

		//记录鼠标左键是否按下，按下为true,初始值为false
		m_bMouseDown = false;
		break;
	case 2://功能待定：暂时不做处理
		break;

	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, (char**)argv); //对GLUT进行初始化 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //设置模式为：双缓冲，深度缓存区
	glutInitWindowPosition(110, 120);//窗口位置
	glutInitWindowSize(WIDTH, HEIGHT);//窗口大小
	glutCreateWindow("project");  //创建窗口

	glewExperimental = GL_TRUE;
	GLenum err = glewInit(); //The OpenGL Extension Wrangler Library (GLEW) is 
	if (GLEW_OK != err) {
		std::cerr << "Error: " << glewGetString(err) << std::endl;
	}

	init();//初始化函数
	glutDisplayFunc(display);    // 重新绘制事件 
	//-------------注册回调函数---------------------//
	glutKeyboardFunc(KeyboardEvent); // 键盘事件
	glutMouseFunc(MouseEvent);       //鼠标事件
	glutReshapeFunc(ReshapeEvent);   //窗口大小发生变化事件
	glutMotionFunc(MotionEvent);     //鼠标移动事件
	glutIdleFunc(IdleEvent);         //空闲处理事件
	glutEntryFunc(MouseEntry);       //检测鼠标进入或离开窗口
	{
	glutCreateMenu(MenuEvent);       //创建菜单
	glutAddMenuEntry("复位", 1);     //菜单项1
	glutAddMenuEntry("待定", 2);     //菜单项2
	glutAttachMenu(GLUT_RIGHT_BUTTON);//鼠标右键按下弹出菜单
	}
	glutMainLoop();                  //调用已注册的回调函数 
	return 0;
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
