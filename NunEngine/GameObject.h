#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include <string>
#include "Globals.h"
#include "ModuleGameObject.h"

class GameObject 
{
friend void ModuleGameObject::SetParent(GameObject* child, GameObject* parent);

public:
	GameObject(const uint _id, const char* name);
	~GameObject();

	bool			Update();

	GameObject*		GetParent() const;
	void			Enable();
	void			Disable();
	const bool		IsActive();

public:
	std::string tag;
	std::vector<GameObject*> children;
	//std::vector<Component*> components;

private:
	uint id;
	GameObject* parent;	
	bool active;		
};

#endif __GAMEOBJECT_H__
