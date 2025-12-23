#include "GameObject.h"
#include <glm/gtc/matrix_transform.hpp>

GameObject::GameObject(glm::vec2 pos, glm::vec2 size)
	: position(pos), size(size), velocity(glm::vec2())
{
}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, glm::vec2 velocity)
	: position(pos), size(size), velocity(velocity)
{
}

glm::mat4 GameObject::getModelMatrix() const
{
	glm::mat4 model{ 1.0f };
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::scale(model, glm::vec3(size, 1.0f));
	return model;
}