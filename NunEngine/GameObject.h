#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include <string>
#include "Globals.h"
#include "ModuleGameObject.h"

class GameObject 
{
public:
	GameObject(const uint _id, const char* name);
	~GameObject();

	bool Update();
	GameObject* GetParent() const;

public:
	std::string tag;
	std::vector<GameObject*> children;
	//std::vector<Component*> components;

private:
	uint id;
	GameObject* parent;	
	bool active;
	

	friend bool ModuleGameObject::SetParent(GameObject* child, GameObject* parent);
};

#endif __GAMEOBJECT_H__
