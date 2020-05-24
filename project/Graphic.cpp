#include "Graphic.h"



GraphicRenderer::GraphicRenderer(Shader shader)
{
	this->shader = shader;
	this->initRenderData();
}

GraphicRenderer::~GraphicRenderer()
{
	glDeleteVertexArrays(1, &this->quadVAO);
	glDeleteVertexArrays(1, &this->cubeVAO);
}

void GraphicRenderer::SetShader(Shader shader)
{
	this->shader = shader;
}

void GraphicRenderer::DrawSprite(int modetype,glm::mat4 modelview, Texture2D texture, glm::vec3 position, glm::vec3 size, glm::vec3 rotate, glm::vec3 color, Texture2D *texture2, Texture2D *texture3 )
{
	// Prepare transformations
	this->shader.Use();
	glm::mat4 model = modelview;
	model = glm::translate(model, glm::vec3(position));  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)


	model = glm::rotate(model, rotate[0], glm::vec3(1.0f, 0.0f, 0.0f)); // Then rotate
	model = glm::rotate(model, rotate[1], glm::vec3(0.0f, 1.0f, 0.0f)); // Then rotate
	model = glm::rotate(model, rotate[2], glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
	

	model = glm::scale(model, glm::vec3(size)); // Last scale

	this->shader.SetMatrix4("model", model);

	// Render textured quad
	this->shader.SetVector3f("spriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	if (texture2 != nullptr)
	{
		glActiveTexture(GL_TEXTURE1);
		texture2->Bind();
	}
	if (texture3 != nullptr)
	{
		glActiveTexture(GL_TEXTURE2);
		texture3->Bind();
	}
	
	if (modetype == 0)//四边形
	{
		glBindVertexArray(this->quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	
	if (modetype == 1)//正方体
	{
		glBindVertexArray(this->cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	if (modetype == 2)//球体
	{
		glBindVertexArray(this->shpereVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shpereVEO);
		//glDrawArrays(GL_TRIANGLES, 0, 51*51);
		glDrawElements(GL_TRIANGLES, 50*50 * 6, GL_UNSIGNED_INT, (void *)0);
	}
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D,0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void GraphicRenderer::initRenderData()
{   


	//1   立方体贴图  圆心在正中心
	{
		GLuint cubeVBO;
		GLfloat cubevertices[] = {
			// Pos                 // Tex
			-0.5, -0.5, -0.5,  0.0, 0.0,  0.0,  0.0, -1.0,
			 0.5, -0.5, -0.5,  1.0, 0.0,  0.0,  0.0, -1.0,
			 0.5,  0.5, -0.5,  1.0, 1.0,  0.0,  0.0, -1.0,
			 0.5,  0.5, -0.5,  1.0, 1.0,  0.0,  0.0, -1.0,
			-0.5,  0.5, -0.5,  0.0, 1.0,  0.0,  0.0, -1.0,
			-0.5, -0.5, -0.5,  0.0, 0.0,  0.0,  0.0, -1.0,
								  	  	    
			-0.5, -0.5,  0.5,  0.0, 0.0,  0.0,  0.0,  1.0,
			 0.5, -0.5,  0.5,  1.0, 0.0,  0.0,  0.0,  1.0,
			 0.5,  0.5,  0.5,  1.0, 1.0,  0.0,  0.0,  1.0,
			 0.5,  0.5,  0.5,  1.0, 1.0,  0.0,  0.0,  1.0,
			-0.5,  0.5,  0.5,  0.0, 1.0,  0.0,  0.0,  1.0,
			-0.5, -0.5,  0.5,  0.0, 0.0,  0.0,  0.0,  1.0,
								  	       
			-0.5,  0.5,  0.5,  1.0, 0.0, -1.0,  0.0,  0.0,
			-0.5,  0.5, -0.5,  1.0, 1.0, -1.0,  0.0,  0.0,
			-0.5, -0.5, -0.5,  0.0, 1.0, -1.0,  0.0,  0.0,
			-0.5, -0.5, -0.5,  0.0, 1.0, -1.0,  0.0,  0.0,
			-0.5, -0.5,  0.5,  0.0, 0.0, -1.0,  0.0,  0.0,
			-0.5,  0.5,  0.5,  1.0, 0.0, -1.0,  0.0,  0.0,
								  	       
			 0.5,  0.5,  0.5,  1.0, 0.0,  1.0,  0.0,  0.0,
			 0.5,  0.5, -0.5,  1.0, 1.0,  1.0,  0.0,  0.0,
			 0.5, -0.5, -0.5,  0.0, 1.0,  1.0,  0.0,  0.0,
			 0.5, -0.5, -0.5,  0.0, 1.0,  1.0,  0.0,  0.0,
			 0.5, -0.5,  0.5,  0.0, 0.0,  1.0,  0.0,  0.0,
			 0.5,  0.5,  0.5,  1.0, 0.0,  1.0,  0.0,  0.0,
								  		     
			-0.5, -0.5, -0.5,  0.0, 1.0,  0.0, -1.0,  0.0,
			 0.5, -0.5, -0.5,  1.0, 1.0,  0.0, -1.0,  0.0,
			 0.5, -0.5,  0.5,  1.0, 0.0,  0.0, -1.0,  0.0,
			 0.5, -0.5,  0.5,  1.0, 0.0,  0.0, -1.0,  0.0,
			-0.5, -0.5,  0.5,  0.0, 0.0,  0.0, -1.0,  0.0,
			-0.5, -0.5, -0.5,  0.0, 1.0,  0.0, -1.0,  0.0,
								  		     
			-0.5,  0.5, -0.5,  0.0, 1.0,  0.0,  1.0,  0.0,
			 0.5,  0.5, -0.5,  1.0, 1.0,  0.0,  1.0,  0.0,
			 0.5,  0.5,  0.5,  1.0, 0.0,  0.0,  1.0,  0.0,
			 0.5,  0.5,  0.5,  1.0, 0.0,  0.0,  1.0,  0.0,
			-0.5,  0.5,  0.5,  0.0, 0.0,  0.0,  1.0,  0.0,
			-0.5,  0.5, -0.5,  0.0, 1.0,  0.0,  1.0,  0.0
		};


		glGenVertexArrays(1, &this->cubeVAO);
		glBindVertexArray(this->cubeVAO);
		glGenBuffers(1, &cubeVBO);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubevertices), cubevertices, GL_STATIC_DRAW);


		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}



	//2 正方形  圆心在正中心
	{
		//正方形贴图
		GLuint VBO;
		GLfloat vertices[] = {
			// Pos            // Tex
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
												  	    
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f
		};

	    
		glGenVertexArrays(1, &this->quadVAO);
		glBindVertexArray(this->quadVAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}



	// 3 球体 圆心在正中心
	//{

	//	
	//		//X=OPcos=rsinφcosθ
	//	 	//Y = OPsin = rsinφsinθ
	//		//Z = rcosφ

	//		GLfloat m_r = 1.0;
	//		int col = 40, row = 30;
	//		GLfloat vertices[40 * 30][5];
	//		GLuint indexs[(40 - 1)*(30 - 1) * 6];
	//		GLuint index_num = 0;
	//		GLfloat ang1 = 0, ang2 = 0;
	//		for (int i = 0; i < row; i++)
	//		{

	//			for (int j = 0; j < col; j++)
	//			{
	//				vertices[i*col + j][0] = m_r * sin( ang1 / 180.0 * 3.1415926) * cos( ang2 / 180.0 * 3.1415926 );
	//				vertices[i*col + j][1] =  m_r * cos( ang1 / 180.0 * 3.1415926 );
	//				vertices[i*col + j][2] = m_r * sin( ang1 / 180.0 * 3.1415926) * sin( ang2 / 180.0 * 3.1415926 );
	//				vertices[i*col + j][3] = (GLfloat)i / (GLfloat)row;
	//				vertices[i*col + j][4] = (GLfloat)j / (GLfloat)col;
	//				ang2 += 360.0 / GLfloat(col);

	//				if (i != row - 1 && j != col - 1)
	//				{
	//					indexs[index_num+0] = i * col + j;
	//					indexs[index_num+1] = i * col + j+1;
	//					indexs[index_num+2] = (i+1) * col + j;
	//					indexs[index_num+3] = i * col + j +1;
	//					indexs[index_num+4] = (i + 1) * col + j+1;
	//					indexs[index_num+5] = (i + 1) * col + j;


	//					index_num += 6;
	//				}	
	//			}
	//			ang1 += 180.0 / GLfloat(row);
	//		}
	//	GLuint VBO;
	//	glGenVertexArrays(1, &this->shpereVAO);
	//	glBindVertexArray(this->shpereVAO);
	//	glGenBuffers(1, &VBO);
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//	glGenBuffers(1, &this->shpereVEO);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->shpereVEO);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);

	//	glEnableVertexAttribArray(0);
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	//	glEnableVertexAttribArray(1);
	//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//	glEnableVertexAttribArray(3);
	//	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//	glBindVertexArray(0);

	//}


	//4 圆柱
	{
	
		//X=OPcos=rsinφcosθ
		//Y = OPsin = rsinφsinθ
		//Z = rcosφ

		GLfloat m_r = 1.0;
		int latitudeBands = 50;//纬度带
		int longitudeBands = 50;//经度带
		GLfloat vertices[51*51][5];
		int indices[50*50][6];//三角形列表（索引值）
		float PI = 3.1415926;
		for (float latNum = 0; latNum <= latitudeBands; latNum++)
		{
			float lat = latNum * PI / latitudeBands - PI / 2.0;//纬度范围从-π/2到π/2
			float sinLat = sin(lat);
			float cosLat = cos(lat);

			for (float longNum = 0; longNum <= longitudeBands; longNum++)
			{
				float lon = longNum * 2 * PI / (longitudeBands)-PI;//经度范围从-π到π
				float sinLon = sin(lon);
				float cosLon = cos(lon);

				float x = cosLat * cosLon;
				float y = cosLat * sinLon;
				float z = sinLat;
				float u = (longNum / (longitudeBands));
				float v = (latNum / (latitudeBands));
				
				vertices[int(latNum*(latitudeBands + 1) + longNum)][0] = x;
				vertices[int(latNum*(latitudeBands + 1) + longNum)][1] = y;
				vertices[int(latNum*(latitudeBands + 1) + longNum)][2] = z;
				vertices[int(latNum*(latitudeBands + 1) + longNum)][3] = u;
				vertices[int(latNum*(latitudeBands + 1) + longNum)][4] = v;


			}
			
		}
		for (int latNum = 0; latNum < latitudeBands; latNum++) {
			for (int longNum = 0; longNum < longitudeBands; longNum++)
			{
				int first = latNum * (longitudeBands + 1) + longNum;
				int second = first + longitudeBands + 1;

				//第一个三角形的3个点索引
				indices[latNum*latitudeBands + longNum][0] = first;
				indices[latNum*latitudeBands + longNum][1] = first+1;
				indices[latNum*latitudeBands + longNum][2] = second;
				indices[latNum*latitudeBands + longNum][3] = second;
				indices[latNum*latitudeBands + longNum][4] = second +1;
				indices[latNum*latitudeBands + longNum][5] = first +1;
			

			
			}
		}
		GLuint VBO;
		glGenVertexArrays(1, &this->shpereVAO);
		glBindVertexArray(this->shpereVAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &this->shpereVEO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->shpereVEO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}


	//5 圆环
	
}
