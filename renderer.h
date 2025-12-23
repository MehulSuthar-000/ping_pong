#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>
#include "shader.h"

class Renderer
{
public:
	Renderer();
	void draw(const Shader& shader, const glm::mat4& model) const;

private:
	unsigned int VAO;
	unsigned int VBO;
};

#endif // !RENDERER_H

