#ifndef __COMPONENT_TRANSFORM_H__
#define __COMPONENT_TRANSFORM_H__

#include "Component.h"
#include "glm\fwd.hpp"
#include "glm\glm.hpp"


class GameObject;

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* game_object);
	~ComponentTransform();

private:
	//glm::quat		rotation;
	glm::vec3		translation;
	glm::vec3		scale;
	glm::mat4x4		transform_mat;


};

#endif
