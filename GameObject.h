#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>

class GameObject
{
public:
	glm::vec2 position;
	glm::vec2 size;
	glm::vec2 velocity;

	GameObject(glm::vec2 pos, glm::vec2 size);
	GameObject(glm::vec2 pos, glm::vec2 size, glm::vec2 velocity);

	glm::mat4 getModelMatrix() const;

};


#endif // !GAMEOBJECT_H

