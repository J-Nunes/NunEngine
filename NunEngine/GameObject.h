#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include <string>
#include "Globals.h"
#include "ModuleGameObject.h"
#include "Components.h"

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
	const bool		IsActive() const;

public:
	std::string					tag;
	std::vector<GameObject*>	children;
	std::vector<Component*>		components;

private:
	uint						id = 0;
	GameObject*					parent = nullptr;	
	bool						active = true;		
};

#endif __GAMEOBJECT_H__
