#pragma once
#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Texture2D.h"
#include "Shader.h"


class GraphicRenderer
{
public:
	// Constructor (inits shaders/shapes)
	GraphicRenderer(Shader shader);
	// Destructor
	~GraphicRenderer();

	void SetShader(Shader shader);
	// Renders a defined quad textured with given sprite
	void DrawSprite(int modetype, glm::mat4 modelview,Texture2D texture, glm::vec3 position, glm::vec3 size = glm::vec3(10, 10,1.0), glm::vec3 rotate = glm::vec3(0.0f), glm::vec3 color = glm::vec3(1.0f), Texture2D *texture2 = nullptr, Texture2D *texture3 = nullptr);
private:
	// Render state
	Shader shader;
	GLuint quadVAO,cubeVAO, shpereVEO, shpereVAO;
	// Initializes and configures the quad's buffer and vertex attributes
	void initRenderData();
};

#endif
